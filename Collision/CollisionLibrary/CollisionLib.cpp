#include "../../PreCompiled.h"
#include "CollisionLib.h"
#include <math.h>

/************************************************
* Filename:      CollisionLib.cpp
* Date:          06/08/2015
* Mod. Date:     07/10/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       To hold all collisions functions
*************************************************/

/********************************************
* CalculatePlane(): Creates a plane and offset out of three points
*
* Ins: _tpointA, _tpointB, _tpointC
*
* Outs: _tplane
*
* Returns: None
*
* Mod. Date 06/19/2015
* Mod. Initials: MM
********************************************/
void CalculatePlane(tPlane &_tplane, const XMFLOAT3& _tpointA, const XMFLOAT3& _tpointB, const XMFLOAT3& _tpointC)
{
	XMFLOAT3 tPointA = XMFLOAT3( (_tpointB.x - _tpointA.x), (_tpointB.y - _tpointA.y), (_tpointB.z - _tpointA.z) );
	XMFLOAT3 tPointB = XMFLOAT3( (_tpointC.x - _tpointB.x), (_tpointC.y - _tpointB.y), (_tpointC.z - _tpointB.z) );

	FXMVECTOR tVectorA = DirectX::XMLoadFloat3(&tPointA);
	FXMVECTOR tVectorB = DirectX::XMLoadFloat3(&tPointB);
	XMVECTOR tNormal = DirectX::XMLoadFloat3(&_tplane.m_tNormal);

	XMVECTOR tNormalResults = DirectX::XMVector3Cross(tVectorA, tVectorB);

	DirectX::XMStoreFloat3(&_tplane.m_tNormal, tNormalResults);

	tNormal = DirectX::XMLoadFloat3(&_tplane.m_tNormal);
	tNormalResults = DirectX::XMVector3Normalize(tNormal);

	DirectX::XMStoreFloat3(&_tplane.m_tNormal, tNormalResults);
	
	tNormal = DirectX::XMLoadFloat3(&_tplane.m_tNormal);
	FXMVECTOR tVectorPoint = DirectX::XMLoadFloat3(&tPointA);

	XMVECTOR tOffsetResults = DirectX::XMVector3Dot(tNormal, tVectorPoint);

	DirectX::XMStoreFloat3(&_tplane.m_tNormal, tOffsetResults);
}

/********************************************
* IdentifyPointToPlane(): Perform a half-space test. 
*						  Returns 1 if the point is on or in front of the plane.
*						  Returns 2 if the point is behind the plane.						  
*
* Ins: _tplane, _tpoint
*
* Outs: None
*
* Returns: int
*
* Mod. Date 06/19/2015
* Mod. Initials: MM
********************************************/
int IdentifyPointToPlane(const tPlane& _tplane, const XMFLOAT3& _tpoint)
{
	XMVECTOR tNormal = DirectX::XMLoadFloat3(&_tplane.m_tNormal);
	XMVECTOR tPoint = DirectX::XMLoadFloat3(&_tpoint);

	XMVECTOR tNormalResults = DirectX::XMVector3Dot(tNormal, tPoint);
	XMFLOAT3 tPlaneNormal;

	DirectX::XMStoreFloat3(&tPlaneNormal, tNormalResults);

	if (tPlaneNormal.x - _tplane.m_fOffset > 0)
		return 1;
	else
		return 2;
	
}

/********************************************
* IdentifySphereToPlane(): Perform a sphere to point test.
*						   Returns 1 if the sphere is in front of the plane.
*						   Returns 2 if the sphere is behind the plane.
*                          Returns 3 if the sphere is in the middle the plane.
*
* Ins: _tplane, _tsphere
*
* Outs: None
*
* Returns: int
*
* Mod. Date 06/19/2015
* Mod. Initials: MM
********************************************/
int IdentifySphereToPlane(const tPlane& _tplane, const tSphere& _tsphere)
{
	XMVECTOR tNormal = DirectX::XMLoadFloat3(&_tplane.m_tNormal);
	XMVECTOR tCenter = DirectX::XMLoadFloat3(&_tsphere.m_tCenter);

	XMVECTOR tNormalResults = DirectX::XMVector3Dot(tNormal, tCenter);
	XMFLOAT3 tPlaneNormal;

	DirectX::XMStoreFloat3(&tPlaneNormal, tNormalResults);

	if (tPlaneNormal.x - _tplane.m_fOffset > _tsphere.m_fRadius)
		return 1;
	else if (tPlaneNormal.x - _tplane.m_fOffset > -_tsphere.m_fRadius)
		return 2;
	else
		return 3;
}

/********************************************
* IdentifyAABBToPlane(): Performs a AABB-to-plane test.
*						 Returns 1 if the aabb is in front of the plane.
*						 Returns 2 if the aabb is behind the plane.
*						 Returns 3 if the aabb straddles the plane.
*
* Ins: _tplane, _tsphere
*
* Outs: None
*
* Returns: int
*
* Mod. Date 06/19/2015
* Mod. Initials: MM
********************************************/
int IdentifyAABBtoPlane(const tPlane& _tplane, const tAABB& _taabb)
{
	XMVECTOR tMin = XMLoadFloat3(&_taabb.m_tMin);
	XMVECTOR tMax = XMLoadFloat3(&_taabb.m_tMax);
	XMFLOAT3 tCenterPoint;

	DirectX::XMStoreFloat3(&tCenterPoint, (tMin + tMax ) * 0.5f);

	XMVECTOR tCenter = XMLoadFloat3(&tCenterPoint);
	XMFLOAT3 tExtent;

	DirectX::XMStoreFloat3(&tExtent, ( tMax - tCenter ));

	float fRadius = (tExtent.x * abs(_tplane.m_tNormal.x)) + (tExtent.y * abs(_tplane.m_tNormal.y)) + (tExtent.z * abs(_tplane.m_tNormal.z));

	tSphere tSphere;
	tSphere.m_tCenter = tCenterPoint;
	tSphere.m_fRadius = fRadius;

	return IdentifySphereToPlane(_tplane, tSphere);
}

/********************************************
* BuildFrustum(): Calculates the corner points and planes of the frustum based upon input values.
*						 Should call ComputePlane.
*
* Ins: _fFov, _fNearDist, _fFarDist, _fRatio, _tcamXform
*
* Outs: _tfrustum
*
* Returns: None
*
* Mod. Date 06/16/2015
* Mod. Initials: MM
********************************************/
void BuildFrustum(tFrustum& _tfrustum, float _fFov, float _fNearDist, float _fFarDist, float _fRatio, const XMFLOAT4X4& _tcamXform)
{

}

/********************************************
* FrustumToSphere(): Perform a Sphere-to-Frustum check. 
*					 Returns true if the sphere is inside. 
*					 False if not.
*
* Ins: _tfrustum, _tsphere
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 06/16/2015
* Mod. Initials: MM
********************************************/
bool FrustumToSphere(const tFrustum& _tfrustum, const tSphere& _tsphere)
{
	return false;
}

/********************************************
* FrustumToAABB(): Perform a AABB-to-Frustum check.
*				   Returns true if the AABB is inside.
*				   False if not.
*
* Ins: _tfrustum, _taabb
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 06/16/2015
* Mod. Initials: MM
********************************************/
bool FrustumToAABB(const tFrustum& _tfrustum, const tAABB& _taabb)
{
	return false;
}

/********************************************
* AABBtoAABB(): Returns true if the AABBs collide. 
*				False if not.
*
* Ins: _taabbA, _taabbB
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 06/16/2015
* Mod. Initials: MM
********************************************/
bool AABBtoAABB(const tAABB& _taabbA, const tAABB& _taabbB)
{
	if (_taabbA.m_tMax.x < _taabbB.m_tMin.x || _taabbA.m_tMin.x > _taabbB.m_tMax.x)
	{
		return false;
	}
	else if (_taabbA.m_tMax.y < _taabbB.m_tMin.y || _taabbA.m_tMin.y > _taabbB.m_tMax.y)
	{
		return false;
	}
	else if (_taabbA.m_tMax.z < _taabbB.m_tMin.z || _taabbA.m_tMin.z > _taabbB.m_tMax.z)
	{
		return false;
	}
	else
		return true;
}

/********************************************
* SphereToSphere(): Returns true if the Spheres collide. 
*					False if not.
*
* Ins: _tsphereA, _tsphereB
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
bool SphereToSphere(const tSphere& _tsphereA, const tSphere& _tsphereB)
{
	//Get the combined radius of both sphere's center
	float fCombinedRadius = _tsphereA.m_fRadius + _tsphereB.m_fRadius;

	//Get the distance between both sphere's center
	XMVECTOR tCenterA = XMLoadFloat3(&_tsphereA.m_tCenter);
	XMVECTOR tCenterB = XMLoadFloat3(&_tsphereB.m_tCenter);
	XMFLOAT3 tDistance;

	DirectX::XMStoreFloat3(&tDistance, tCenterA - tCenterB);

	//Get the distance magnitude
	XMVECTOR tDist = XMLoadFloat3(&tDistance);
	XMVECTOR tLength = XMVector3Length(tDist);
	XMFLOAT3 tMagnitude;

	DirectX::XMStoreFloat3(&tMagnitude, tLength);

	//Check to see if the distance's magnitude is less than the combined radius
	//If it is, return true. Else return false.
	if (tMagnitude.x < fCombinedRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/********************************************
* SphereToAABB(): Returns true if the sphere collides with the AABB. 
*				  False if not.
*
* Ins: _tsphere, _taabb
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
bool SphereToAABB(const tSphere& _tsphere, const tAABB& _taabb)
{
	//Get the closestpoint of the aabb from the sphere's center
	XMFLOAT3 tClosestPoint = XMFLOAT3(0, 0, 0);

	ClosestPointOnAABB(tClosestPoint, _tsphere.m_tCenter, _taabb);

	//Find the distance from the closest point to the sphere center
	XMVECTOR tAABBClosest = XMLoadFloat3(&tClosestPoint);
	XMVECTOR tCenter = XMLoadFloat3(&_tsphere.m_tCenter);
	XMFLOAT3 tDistance;

	DirectX::XMStoreFloat3(&tDistance, tAABBClosest - tCenter);

	//Get the magnitude of the distance
	XMVECTOR tDistanceResults = XMLoadFloat3(&tDistance);
	XMVECTOR tLength = XMVector3Length(tDistanceResults);
	XMFLOAT3 tMagnitude;

	DirectX::XMStoreFloat3(&tMagnitude, tLength);

	//Check to see if the distance's magnitude is less than the radius
	//If it is, return true. Else return false.
	if (tMagnitude.x <= _tsphere.m_fRadius)
	{
		return true;
	}
	else
		return false;
}

/********************************************
* CapsuleToSphere(): Returns true if the Capsule collides with the Sphere.
*				     False if not.
*
* Ins: _tcapsule, _tsphere
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 07/03/2015
* Mod. Initials: MM
********************************************/
bool CapsuleToSphere(const tCapsule& _tcapsule, const tSphere& _tsphere)
{
	//Find the Closest Point of Capsule A using the Sphere's Center.
	XMFLOAT3 tTestPoint = ClosestPointLineSegment(_tcapsule.m_tsegment, _tsphere.m_tCenter);

	//Get the combined radius of the capsule and the sphere
	float fCombinedRadius = _tcapsule.m_fRadius + _tsphere.m_fRadius;

	//Get the distance from the test point to the sphere center
	XMVECTOR tCenterA = XMLoadFloat3(&tTestPoint);
	XMVECTOR tCenterB = XMLoadFloat3(&_tsphere.m_tCenter);
	XMFLOAT3 tDistance;

	DirectX::XMStoreFloat3(&tDistance, tCenterA - tCenterB);

	//Find the magnitude of the distance
	XMVECTOR tDist = XMLoadFloat3(&tDistance);
	XMVECTOR tLength = XMVector3Length(tDist);
	XMFLOAT3 tMagnitude;

	DirectX::XMStoreFloat3(&tMagnitude, tLength);

	//Check to see if the distance's magnitude is less than the combined radius
	//If it is, return true. Else return false.
	if (tMagnitude.x < fCombinedRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/********************************************
* CapsuleToAABB(): Returns true if the Capsule collides with the AABB.
*				   False if not.
*
* Ins: _tcapsule, _taabb
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 06/21/2015
* Mod. Initials: MM
********************************************/
bool CapsuleToAABB(const tCapsule& _tcapsule, const tAABB& _taabb)
{
	//Get AABB CenterPoint
	XMVECTOR tMin = XMLoadFloat3(&_taabb.m_tMin);
	XMVECTOR tMax = XMLoadFloat3(&_taabb.m_tMax);
	XMFLOAT3 tAABBCenterPoint;

	DirectX::XMStoreFloat3(&tAABBCenterPoint, (tMin + tMax) * 0.5f);

	//Get the AABB Half-Extents
	XMVECTOR tAABBCenter = XMLoadFloat3(&tAABBCenterPoint);
	XMFLOAT3 tAABBHalfExtent;

	DirectX::XMStoreFloat3(&tAABBHalfExtent, tMax - tAABBCenter);

	//Get segment midpoint
	XMVECTOR tStartPoint = XMLoadFloat3(&_tcapsule.m_tsegment.m_tStart);
	XMVECTOR tEndPoint = XMLoadFloat3(&_tcapsule.m_tsegment.m_tEnd);
	XMFLOAT3 tMidPoint;

	DirectX::XMStoreFloat3(&tMidPoint, (tStartPoint + tEndPoint) * 0.5f);

	//Get HalfLength
	XMVECTOR tMid = XMLoadFloat3(&tMidPoint);
	XMFLOAT3 tHalfLength;

	DirectX::XMStoreFloat3(&tHalfLength, tEndPoint - tMid);

	//Midpoint = midpoint - center
	DirectX::XMStoreFloat3(&tMidPoint, tMid - tAABBCenter);

	//Try world coordinates axes as separating axes
	float adx = abs(tHalfLength.x);

	if (abs(tMidPoint.x) > tAABBHalfExtent.x + adx + _tcapsule.m_fRadius)
	{
		return false;
	}

	float ady = abs(tHalfLength.y);

	if (abs(tMidPoint.y) > tAABBHalfExtent.y + ady + _tcapsule.m_fRadius)
	{
		return false;
	}

	float adz = abs(tHalfLength.z);

	if (abs(tMidPoint.z) > tAABBHalfExtent.z + adz + _tcapsule.m_fRadius)
	{
		return false;
	}

	//Add in an Epsilon term to counteract arithmetic errors when segment is (near) parallel to a coordinate axis
	adx += FLT_EPSILON;
	ady += FLT_EPSILON;
	adz += FLT_EPSILON;
	
	//Try cross products of segment direction vector with coordinate axes
	if (abs(tMidPoint.y * tHalfLength.z - tMidPoint.z * tHalfLength.y) > tAABBHalfExtent.y * adz + tAABBHalfExtent.z * ady + _tcapsule.m_fRadius)
	{
		return false;
	}

	if (abs(tMidPoint.z * tHalfLength.x - tMidPoint.x * tHalfLength.z) > tAABBHalfExtent.x * adz + tAABBHalfExtent.z * adx + _tcapsule.m_fRadius)
	{
		return false;
	}

	if (abs(tMidPoint.x * tHalfLength.y - tMidPoint.y * tHalfLength.x) > tAABBHalfExtent.x * ady + tAABBHalfExtent.y * adx + _tcapsule.m_fRadius)
	{
		return false;
	}

	//No separating axis found, segment must be overlapping AABB
	return true;
}

/********************************************
* CapsuleToCapsule(): Returns true if the Capsule collides with another Capsule.
*				      False if not.
*
* Ins: _tcapsuleA, _tcapsuleB
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
bool CapsuleToCapsule(const tCapsule& _tcapsuleA, const tCapsule& _tcapsuleB)
{
	float fDistanceA, fDistanceB;
	XMFLOAT3 tClosestPointA, tClosestPointB;

	float fDistanceTo = ClosestPointSegmentSegment(_tcapsuleA.m_tsegment, _tcapsuleB.m_tsegment, fDistanceA, fDistanceB, tClosestPointA, tClosestPointB);

	float fCombinedRadius = _tcapsuleA.m_fRadius + _tcapsuleB.m_fRadius;
	float tSquaredRadius = fCombinedRadius * fCombinedRadius;

	if (fDistanceTo <= tSquaredRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/********************************************
* RayToSphere(): Returns true if a Ray has collided with a Sphere.
*				 False if not.
*
* Ins: _tray, _tsphere
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 07/10/2015
* Mod. Initials: MM
********************************************/
bool RayToSphere(const tRay& _tray, const tSphere& _tsphere)
{
	//Find the distance between the closest point and the sphere's center
	XMVECTOR tRayStartPoint = XMLoadFloat3(&_tray.m_tStartPoint);
	XMVECTOR tSphereCenter = XMLoadFloat3(&_tsphere.m_tCenter);
	XMFLOAT3 tLengthVector;

	DirectX::XMStoreFloat3(&tLengthVector, tSphereCenter - tRayStartPoint);

	//Now calculate the dot between the distance and the direction
	XMVECTOR tDirection = XMLoadFloat3(&_tray.m_tRayDirection);
	XMVECTOR tDistance = XMLoadFloat3(&tLengthVector);
	XMVECTOR tDirectDot = XMVector3Dot(tDistance, tDirection);
	XMFLOAT3 tDirectDotResult;

	DirectX::XMStoreFloat3(&tDirectDotResult, tDirectDot);

	//Now calculate the dot of the distance by itself
	XMVECTOR tDistDot = XMVector3Dot(tDistance, tDistance);
	XMFLOAT3 tDistDotResult;

	DirectX::XMStoreFloat3(&tDistDotResult, tDistDot);

	//Now find 'c' by using the distance dot result -  the squared radius
	float fSquaredRadius = _tsphere.m_fRadius * _tsphere.m_fRadius;
	float c = tDistDotResult.x - fSquaredRadius;

	//If the the ray's startpoint is outside of the sphere AND if the ray is pointing away from the sphere, return false
	if (c > 0.0f && tDirectDotResult.x > 0.0f)
	{
		return false;
	}

	//Now calculate the discriminant, and see if its negative, if so return false
	float fDiscriminant = tDirectDotResult.x * tDirectDotResult.x - c;

	if (fDiscriminant < 0.0f)
	{
		return false;
	}

	//If you made it to this point, your intersecting with the sphere
	return true;
	
}

/********************************************
* RayToAABB(): Returns true if a Ray has collided with an AABB.
*			   False if not.
*
* Ins: _tray, _tsphere
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 07/13/2015
* Mod. Initials: MM
********************************************/
bool RayToAABB(const tRay& _tray, const tAABB& _taabb)
{
	//Setup the functions min and max
	float fmin = 0.0f;
	float fmax = FLT_MAX;

	//Store all variables passed in into float arrays for the for loop
	float fMax[3], fMin[3], fDirection[3], fStartPoint[3];

	//Max setup
	fMax[0] = _taabb.m_tMax.x;
	fMax[1] = _taabb.m_tMax.y;
	fMax[2] = _taabb.m_tMax.z;

	//Min setup
	fMin[0] = _taabb.m_tMin.x;
	fMin[1] = _taabb.m_tMin.y;
	fMin[2] = _taabb.m_tMin.z;

	//Direction setup
	fDirection[0] = _tray.m_tRayDirection.x;
	fDirection[1] = _tray.m_tRayDirection.y;
	fDirection[2] = _tray.m_tRayDirection.z;

	//Startpoint setup
	fStartPoint[0] = _tray.m_tStartPoint.x;
	fStartPoint[1] = _tray.m_tStartPoint.y;
	fStartPoint[2] = _tray.m_tStartPoint.z;

	//Now check on all 3 axises for collision
	for (unsigned int i = 0; i < 3; i++)
	{
		//if the absolute value of a direction is less than the float_epsilon,
		if (abs(fDirection[i]) < FLT_EPSILON)
		{
			//the ray is parallel to the axis, return false
			if (fStartPoint[i] < fMin[i] || fStartPoint[i] > fMax[i])
			{
				return false;
			}
		}
		else
		{
			//Compute intersection of the ray with the near and far plane of the axis
			float fObjectOrientedDirection = 1.0f / fDirection[i];

			float fNear = (fMin[i] - fStartPoint[i]) * fObjectOrientedDirection;
			float fFar = (fMax[i] - fStartPoint[i]) * fObjectOrientedDirection;

			//Make fNear be intersecting with fFar
			if (fNear > fFar)
			{
				swap(fNear, fFar);
			}

			//Compute the intersection of axis intersection intervals
			fmin = max(fmin, fNear);
			fmax = min(fmax, fFar);

			//If fmin is greater than fmax, return false
			if (fmin > fmax)
			{
				return false;
			}
		}
	}

	//If it made it to this point, the Ray has collided on all Axises, return true
	return true;
}

/********************************************
* ClosestPointOnAABB(): Finds the closest point on an aabb from a given point
*
* Ins: _tPointA, _taabb
*
* Outs: _tClosestPoint
*
* Returns: None
*
* Mod. Date 06/19/2015
* Mod. Initials: MM
********************************************/
void ClosestPointOnAABB(XMFLOAT3& _tClosestPoint, const XMFLOAT3& _tPointA, const tAABB& _taabb)
{
	//Use the pointA passed in to check against the min and max of the aabb
	XMVECTOR tPoint = XMLoadFloat3(&_tPointA);
	XMFLOAT3 tTempPoint;

	DirectX::XMStoreFloat3(&tTempPoint, tPoint);

	//On each axis, see if the point is either greater than the man or less than the min
	//If its greater than the max, set the point's axis to the max.
	//If its less than the min, set the point's axis to the min.
	//X Axis
	if (tTempPoint.x < _taabb.m_tMin.x)
	{
		tTempPoint.x = _taabb.m_tMin.x;
	}

	if (tTempPoint.x > _taabb.m_tMax.x)
	{
		tTempPoint.x = _taabb.m_tMax.x;
	}

	//Y Axis
	if (tTempPoint.y < _taabb.m_tMin.y)
	{
		tTempPoint.y = _taabb.m_tMin.y;
	}

	if (tTempPoint.y > _taabb.m_tMax.y)
	{
		tTempPoint.y = _taabb.m_tMax.y;
	}

	//Z Axis
	if (tTempPoint.z < _taabb.m_tMin.z)
	{
		tTempPoint.z = _taabb.m_tMin.z;
	}

	if (tTempPoint.z > _taabb.m_tMax.z)
	{
		tTempPoint.z = _taabb.m_tMax.z;
	}

	//Store the point into the closestpoint variable and send it back.
	XMVECTOR tClosestPoint = XMLoadFloat3(&tTempPoint);

	DirectX::XMStoreFloat3(&_tClosestPoint, tClosestPoint);
}

/********************************************
* ClosestPointLineSegment(): Finds the closest point on an capsule from a given point
*
* Ins: _tcapsule, _tTestPoint
*
* Outs: None
*
* Returns: XMFLOAT3
*
* Mod. Date 06/21/2015
* Mod. Initials: MM
********************************************/
XMFLOAT3 ClosestPointLineSegment(tSegment _tsegment, XMFLOAT3 _tTestPoint)
{
	//Calculate a vector from the start point to the end point
	XMVECTOR tStartPoint = XMLoadFloat3(&_tsegment.m_tStart);
	XMVECTOR tEndPoint = XMLoadFloat3(&_tsegment.m_tEnd);
	XMFLOAT3 tLengthVector;

	DirectX::XMStoreFloat3(&tLengthVector, tEndPoint - tStartPoint);

	//Normalize the LengthVector to find the Normal
	XMVECTOR tLengVector = XMLoadFloat3(&tLengthVector);
	XMVECTOR tNormal = DirectX::XMVector3Normalize(tLengVector);

	//Calculate the vector from the start point to the test point
	XMVECTOR tTestVector = XMLoadFloat3(&_tTestPoint);
	XMFLOAT3 tVector;

	DirectX::XMStoreFloat3(&tVector, tTestVector - tStartPoint);

	//Calculate the dot product of the normal and the vector
	XMVECTOR tVec = XMLoadFloat3(&tVector);
	XMVECTOR tDotProduct = XMVector3Dot(tNormal, tVec);

	XMFLOAT3 tDot;
	XMFLOAT3 tClosestPoint;

	DirectX::XMStoreFloat3(&tDot, tDotProduct);

	//Check to see if the Dot Product of Normal and Vector is less than zero if so
	//The Closest Point is the Start Point
	if (tDot.x < 0.0f)
	{
		tClosestPoint = _tsegment.m_tStart;
		return tClosestPoint;
	}

	//Check to see if the Dot Product of Normal and Vector is greater than length of the line segment. If so
	//The Closest Point is the End Point
	XMVECTOR tLeng = XMVector3Length(tLengVector);
	XMFLOAT3 tLength;

	DirectX::XMStoreFloat3(&tLength, tLeng);

	if (tDot.x > tLength.x)
	{
		tClosestPoint = _tsegment.m_tEnd;
		return tClosestPoint;
	}

	//Scale the normal to get Normal's Prime
	XMFLOAT3 tNormalPrime;

	DirectX::XMStoreFloat3(&tNormalPrime, tNormal * tDotProduct);

	//Add Normal Prime with the start point to get the closest point
	XMVECTOR tNPrime = XMLoadFloat3(&tNormalPrime);

	DirectX::XMStoreFloat3(&tClosestPoint, tStartPoint + tNPrime);

	//Return the ClosestPoint
	return tClosestPoint;
}

/********************************************
* ClosestPointOnRay(): Finds the closest point on a Ray from a given point
*
* Ins: _tray, _tTestPoint
*
* Outs: None
*
* Returns: XMFLOAT3
*
* Mod. Date 07/10/2015
* Mod. Initials: MM
********************************************/
XMFLOAT3 ClosestPointOnRay(tRay _tray, XMFLOAT3 _tTestPoint)
{
	//Calculate a vector from the start point to the end point
	XMVECTOR tStartPoint = XMLoadFloat3(&_tray.m_tStartPoint);
	XMVECTOR tTestPoint = XMLoadFloat3(&_tTestPoint);
	XMFLOAT3 tLengthVector;

	DirectX::XMStoreFloat3(&tLengthVector, tTestPoint - tStartPoint);

	//Now Calculate the Dot Product of the Normal and the LenghVector
	XMVECTOR tLengVector = XMLoadFloat3(&tLengthVector);
	XMVECTOR tNormal = XMLoadFloat3(&_tray.m_tRayDirection);
	XMVECTOR tDot = XMVector3Dot(tNormal, tLengVector);
	XMFLOAT3 tDotResults;

	DirectX::XMStoreFloat3(&tDotResults, tDot);

	//Now check to see if the DotResults is less than Zero.
	//If so then the ClosetPoint is the StartPoint of the Ray.
	XMFLOAT3 tClosestPoint;

	if (tDotResults.x < 0.0f)
	{
		tClosestPoint = _tray.m_tStartPoint;
		return tClosestPoint;
	}

	// If it's not, than find the normal's prime
	XMFLOAT3 tNormalPrime;

	DirectX::XMStoreFloat3(&tNormalPrime, tNormal * tDot);

	//Add Normal Prime with the start point to get the closest point
	XMVECTOR tNPrime = XMLoadFloat3(&tNormalPrime);

	DirectX::XMStoreFloat3(&tClosestPoint, tStartPoint + tNPrime);

	//Return the ClosestPoint
	return tClosestPoint;
}

/********************************************
* ClosestPointSegmentSegment(): Finds the closestpoint between two segments.
*								It also return the distance between both segments
*
* Ins: _tSegmentA, _tSegmentB, _fDistanceA, _fDistanceB, _tClosestPointA, _tClosestPointB
*
* Outs: None
*
* Returns: float
*
* Mod. Date 07/10/2015
* Mod. Initials: MM
********************************************/
float ClosestPointSegmentSegment(tSegment _tSegmentA, tSegment _tSegmentB, float& _fDistanceA, float& _fDistanceB, XMFLOAT3& _tClosestPointA, XMFLOAT3& _tClosestPointB)
{
	//Find the direction of Segment A
	XMVECTOR tStartPointA = XMLoadFloat3(&_tSegmentA.m_tStart);
	XMVECTOR tEndPointA = XMLoadFloat3(&_tSegmentA.m_tEnd);
	XMFLOAT3 tDirectionA;

	DirectX::XMStoreFloat3(&tDirectionA, tEndPointA - tStartPointA);

	//Find the direction of Segment B
	XMVECTOR tStartPointB = XMLoadFloat3(&_tSegmentB.m_tStart);
	XMVECTOR tEndPointB = XMLoadFloat3(&_tSegmentB.m_tEnd);
	XMFLOAT3 tDirectionB;

	DirectX::XMStoreFloat3(&tDirectionB, tEndPointB - tStartPointB);

	//Find the range between both segment's startpoints
	XMFLOAT3 tRange;

	DirectX::XMStoreFloat3(&tRange, tStartPointA - tStartPointB);

	//Find the Squared Length of Segment A
	XMVECTOR tDirectA = XMLoadFloat3(&tDirectionA);
	XMVECTOR tDotA = XMVector3Dot(tDirectA, tDirectA);
	XMFLOAT3 tSquaredLengthA;

	DirectX::XMStoreFloat3(&tSquaredLengthA, tDotA);

	//Find the Squared Length of Segment B
	XMVECTOR tDirectB = XMLoadFloat3(&tDirectionB);
	XMVECTOR tDotB = XMVector3Dot(tDirectB, tDirectB);
	XMFLOAT3 tSquaredLengthB;

	DirectX::XMStoreFloat3(&tSquaredLengthB, tDotB);

	//Find f
	XMVECTOR tRanged = XMLoadFloat3(&tRange);
	XMVECTOR tDotF = XMVector3Dot(tDirectB, tRanged);
	XMFLOAT3 tF;

	DirectX::XMStoreFloat3(&tF, tDotF);

	//Check if both segments degenerate into points
	if (tSquaredLengthA.x <= FLT_EPSILON && tSquaredLengthB.x <= FLT_EPSILON)
	{
		//Since both points degenerate, set both distances to 0.0f.
		_fDistanceA = _fDistanceB = 0.0f;

		//Then set both closestpoints to the startpoint of each segment
		_tClosestPointA = _tSegmentA.m_tStart;
		_tClosestPointB = _tSegmentB.m_tStart;

		//Find the dot between both closestpoints and return the results
		XMVECTOR tClosestPointA = XMLoadFloat3(&_tClosestPointA);
		XMVECTOR tClosestPointB = XMLoadFloat3(&_tClosestPointB);
		XMVECTOR tDotResults = XMVector3Dot(tClosestPointA - tClosestPointB, tClosestPointA - tClosestPointB);
		XMFLOAT3 tResults;

		DirectX::XMStoreFloat3(&tResults, tDotResults);

		return tResults.x;
	}

	//If the first segments degenerates into a point ...
	if (tSquaredLengthA.x <= FLT_EPSILON)
	{
		_fDistanceA = 0.0f;
		_fDistanceB = tF.x / tSquaredLengthB.x;
		_fDistanceB = Clamp(_fDistanceB, 0.0f, 1.0f);
	}
	else
	{
		//Find C
		XMVECTOR tDotC = XMVector3Dot(tDirectA, tRanged);
		XMFLOAT3 tC;

		DirectX::XMStoreFloat3(&tF, tDotC);

		//If the second segments degenerates into a point ...
		if (tSquaredLengthB.x < FLT_EPSILON)
		{
			_fDistanceA = -tC.x / tSquaredLengthA.x;
			_fDistanceB = 0.0f;
			_fDistanceA = Clamp(_fDistanceA, 0.0f, 1.0f);
		}
		else
		{
			//The General nondegenerate case starts here
			//Find L
			XMVECTOR tDotL = XMVector3Dot(tDirectA, tDirectB);
			XMFLOAT3 tL;

			DirectX::XMStoreFloat3(&tL, tDotL);

			//Get the Denom
			float fDenom = (tSquaredLengthA.x * tSquaredLengthB.x) - (tL.x * tL.x);

			//If segments are not parallel, compute the closestpoint on L1 and L2
			//and Clamp to segment S1. Else, make Distance A equal Zero.
			if (fDenom != 0.0f)
			{
				_fDistanceA = Clamp(((tL.x * tF.x) - (tC.x * tSquaredLengthB.x)) / fDenom, 0.0f, 1.0f);
			}
			else
			{
				_fDistanceA = 0.0f;
			}

			//Compute point on L2 closest to S1
			_fDistanceB = (tL.x * _fDistanceA + tF.x) / tSquaredLengthB.x;

			//If _fDistanceB less than Zero, Clamp _fDistanceA, else if _fDistanceB is greater than One, Clamp _fDistanceB
			if (_fDistanceB < 0.0f)
			{
				_fDistanceA = -tC.x / tSquaredLengthA.x;
				_fDistanceB = 0.0f;
				_fDistanceA = Clamp(_fDistanceA, 0.0f, 1.0f);
			}
			else if (_fDistanceB > 1.0f)
			{
				_fDistanceA = tL.x - tC.x;
				_fDistanceB = 1.0f;
				_fDistanceA = Clamp(_fDistanceA, 0.0f, 1.0f);
			}
		}
	}

	//Find the _tClosestPointA
	DirectX::XMStoreFloat3(&_tClosestPointA, tStartPointA + tDirectA * _fDistanceA);
	DirectX::XMStoreFloat3(&_tClosestPointB, tStartPointB + tDirectB * _fDistanceB);

	//Find the dot between both closestpoints and return the results
	XMVECTOR tClosestPointA = XMLoadFloat3(&_tClosestPointA);
	XMVECTOR tClosestPointB = XMLoadFloat3(&_tClosestPointB);
	XMVECTOR tDotResults = XMVector3Dot(tClosestPointA - tClosestPointB, tClosestPointA - tClosestPointB);
	XMFLOAT3 tResults;

	DirectX::XMStoreFloat3(&tResults, tDotResults);

	return tResults.x;
}

/********************************************
* Clamp(): Clamps N to lie within the range [Min, Max]
*
* Ins: _fN, _fMin, _fMax
*
* Outs: None
*
* Returns: float
*
* Mod. Date 07/10/2015
* Mod. Initials: MM
********************************************/
float Clamp(float _fN, float _fMin, float _fMax)
{
	if (_fN < _fMin)
	{
		return _fMin;
	}

	if (_fN > _fMax)
	{
		return _fMax;
	}

	return _fN;
}

bool RayPlaneIntersect(float _fPlaneMin, float _fPlaneMax, float _fStart, float _fEnd, float& tbenter, float& tbexit)
{
	float raydir = _fEnd - _fStart;

	// ray parallel to the planes
	if (fabs(raydir) < 0.000000001f)
	{
		// ray parallel to the planes, but ray not inside the planes
		if (_fStart < _fPlaneMin || _fStart > _fPlaneMax)
		{
			return false;
		}
		// ray parallel to the planes, but ray inside the planes
		else
		{
			return true;
		}
	}

	// plane's enter and exit parameters
	float tsenter = (_fPlaneMin - _fStart) / raydir;
	float tsexit = (_fPlaneMax - _fStart) / raydir;

	// order the enter / exit values.
	if (tsenter > tsexit)
	{
		swap(tsenter, tsexit);
	}

	// make sure the plane interval and the current box intersection interval overlap
	if (tbenter > tsexit || tsenter > tbexit)
	{
		// nope. Ray missed the box.
		return false;
	}
	// the plane and current intersection interval overlap
	else
	{
		// update the intersection interval
		tbenter = max(tbenter, tsenter);
		tbexit = min(tbexit, tsexit);
		return true;
	}
}

bool SegmentToAABB(const tAABB& _tAABB, const XMFLOAT3& _xmStart, const XMFLOAT3& _xmEnd, float& _fThisDist, float _fPriorDist)
{
	// initialise to the segment's boundaries. 
	float tenter = 0.0f, texit = 1.0f;

	// test X planes
	if (!RayPlaneIntersect(_tAABB.m_tMin.x, _tAABB.m_tMax.x, _xmStart.x, _xmEnd.x, tenter, texit))
	{
		return false;
	}

	// test Y planes

	if (!RayPlaneIntersect(_tAABB.m_tMin.y, _tAABB.m_tMax.y, _xmStart.y, _xmEnd.y, tenter, texit))
	{
		return false;
	}

	// test Z planes
	if (!RayPlaneIntersect(_tAABB.m_tMin.z, _tAABB.m_tMax.z, _xmStart.z, _xmEnd.z, tenter, texit))
	{
		return false;
	}

	// all intersections in the green. Return the first time of intersection, tenter.
	
	if (tenter > 0 && tenter < _fPriorDist)
	{
		_fThisDist = tenter;
		return true;
	}
	else return false;
}