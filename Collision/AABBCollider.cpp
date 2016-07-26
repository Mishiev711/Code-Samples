#include "../PreCompiled.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

using namespace DirectX;

/************************************************
* Filename:      AABBCollider.cpp
* Date:          06/08/2015
* Mod. Date:     06/20/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       To handle AABB game object's collisions
*************************************************/

/********************************************
* CAABBCollider(): Setup the variables of the AABB class. [ Constructor ]
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
CAABBCollider::CAABBCollider()
{
	m_tInitialAABB.m_tMax = XMFLOAT3(0, 0, 0);
	m_tInitialAABB.m_tMin = XMFLOAT3(0, 0, 0);

	m_tCurrentAABB = m_tInitialAABB;

	m_tPosition = XMFLOAT3(0, 0, 0);
	m_bActive = true;
}

/********************************************
* CAABBCollider(): Setup the variables of the AABB class. [ Overloaded Constructor ]
*
* Ins: _object, _unSize
*
* Outs: None
*
* Returns: None
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
CAABBCollider::CAABBCollider(CObjectBase* _object, unsigned int _unSize)
{
	this->BuildAABB(_object, _unSize);
	m_tPosition = _object->GetPosition();
	m_tCurrentAABB = m_tInitialAABB;
	m_bActive = true;
}

/********************************************
* ~CAABBCollider(): Shutdown the Class [ Destructor ]
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 06/16/2015
* Mod. Initials: MM
********************************************/
CAABBCollider::~CAABBCollider(void)
{

}

/********************************************
* GetType(): Returns the type of Collider the object is.
*
* Ins: None
*
* Outs: None
*
* Returns: eColliderID
*
* Mod. Date 06/16/2015
* Mod. Initials: MM
********************************************/
eColliderID CAABBCollider::GetType() const
{
	return AABB;
}

/********************************************
* GetAABB(): Returns the Collider surrounding the object.
*
* Ins: None
*
* Outs: None
*
* Returns: tAABB
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
tAABB CAABBCollider::GetAABB() const
{
	return m_tCurrentAABB;
}

/********************************************
* GetPosition(): Returns the Position of the Collider.
*
* Ins: None
*
* Outs: None
*
* Returns: XMFLOAT3
*
* Mod. Date 06/16/2015
* Mod. Initials: MM
********************************************/
XMFLOAT3 CAABBCollider::GetPosition() const
{
	return m_tPosition;
}

/********************************************
* SetPosition(): Sets the Position of the Collider.
*
* Ins: _tPosition
*
* Outs: None
*
* Returns: None
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
void CAABBCollider::SetPosition(XMFLOAT3 _tPosition)
{
	m_tPosition = _tPosition;
}

/********************************************
* Update(): Updates the position of the collider
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
void CAABBCollider::Update()
{
	XMVECTOR tMin = XMLoadFloat3(&m_tInitialAABB.m_tMin);
	XMVECTOR tMax = XMLoadFloat3(&m_tInitialAABB.m_tMax);
	XMVECTOR tPosition = XMLoadFloat3(&m_tPosition);

	DirectX::XMStoreFloat3(&m_tCurrentAABB.m_tMax, tMax + (tPosition));
	DirectX::XMStoreFloat3(&m_tCurrentAABB.m_tMin, tMin + (tPosition));
}

/********************************************
* CheckCollisionWith(): If the the Collider is not active, return false
*					    Otherwise, check the type of the other collider and perform the
*					    approperiate collision check. Then return the results.
*					    Also find out which axis did the other collider collided on.
*
* Ins: _other
*
* Outs: _tPosition
*
* Returns: bool
*
* Mod. Date 06/19/2015
* Mod. Initials: MM
********************************************/
bool CAABBCollider::CheckCollisionWith(CICollider* _other) const
{
	if (!this->GetActive())
	{
		return false;
	}

	switch (_other->GetType())
	{
		case AABB:
		{
			CAABBCollider* cOtherCollider = ((CAABBCollider*)(_other));

			if (AABBtoAABB(m_tCurrentAABB, cOtherCollider->m_tCurrentAABB))
			{
				return true;
			}
			else
			{
				return false;
			}

			break;
		}
		case SPHERE:
		{
			CSphereCollider* cOtherCollider = ((CSphereCollider*)(_other));

			if (SphereToAABB(cOtherCollider->GetSphere(), m_tCurrentAABB))
			{
				return true;
			}
			else
			{
				return false;
			}
			break;
		}
		case CAPSULE:
		{
			 CCapsuleCollider* cOtherCollider = ((CCapsuleCollider*)(_other));

			 if (CapsuleToAABB(cOtherCollider->GetCapsule(), m_tCurrentAABB))
			 {
			   return true;
			 }
			 else
			 {
			   return false;
			 }
			 break;
		}
	}

	return false;
}

/********************************************
* ResovleCollision(): After collision is checked from the OnCollisionWith(),
*					  You use the collider passed in to see what type of collider it is and then
*					  perform the case that matches the collider. You're finding the penetration depth
*					  and setting it to the XMFLOAT3 that was passed in.
*
* Ins: _other
*
* Outs: _Position
*
* Returns: None
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
void CAABBCollider::ResolveCollision(XMFLOAT3& _tOffset, CICollider* _other) const
{
	switch (_other->GetType())
	{
		case AABB:
		{
			break;
		}
		case SPHERE:
		{
			CSphereCollider* m_pcOtherCollider = ((CSphereCollider*)(_other));

			tSphere tOtherSphere = m_pcOtherCollider->GetSphere();

			XMFLOAT3 tClosestPoint = XMFLOAT3(0, 0, 0);

			ClosestPointOnAABB(tClosestPoint, tOtherSphere.m_tCenter, m_tCurrentAABB);

			XMVECTOR tAABBClosest = XMLoadFloat3(&tClosestPoint);
			XMVECTOR tCenter = XMLoadFloat3(&tOtherSphere.m_tCenter);
			XMFLOAT3 tDistance;

			DirectX::XMStoreFloat3(&tDistance, tAABBClosest - tCenter);

			XMVECTOR tDistanceResults = XMLoadFloat3(&tDistance);
			XMVECTOR tLength = XMVector3Length(tDistanceResults);
			XMFLOAT3 tMagnitude;

			DirectX::XMStoreFloat3(&tMagnitude, tLength);

			//If the Sphere is inside the AABB, Add the Magnitude to the radius and then 
			//divide the magnitude by the new radius to get the modifier and then divided the
			//distance between the Sphere's center and closest AABB point and the modifier
			if (tOtherSphere.m_tCenter.x < m_tCurrentAABB.m_tMax.x && tOtherSphere.m_tCenter.x > m_tCurrentAABB.m_tMin.x &&
				tOtherSphere.m_tCenter.y < m_tCurrentAABB.m_tMax.y && tOtherSphere.m_tCenter.y > m_tCurrentAABB.m_tMin.y &&
				tOtherSphere.m_tCenter.z < m_tCurrentAABB.m_tMax.z && tOtherSphere.m_tCenter.z > m_tCurrentAABB.m_tMin.z)
			{
				float fExtendedRadius = tMagnitude.x + tOtherSphere.m_fRadius;

				float fVectorModifier = tMagnitude.x / fExtendedRadius;

				if (fVectorModifier == 0)
				{
					if (tClosestPoint.x == 0)
					{
						tClosestPoint.x = -1;
					}
					else
					{
						tClosestPoint.x /= tClosestPoint.x;
					}

					if (tClosestPoint.y == 0)
					{
						tClosestPoint.y = -1;
					}
					else
					{
						tClosestPoint.y /= tClosestPoint.y;
					}

					if (tClosestPoint.z == 0)
					{
						tClosestPoint.z = -1;
					}
					else
					{
						tClosestPoint.z /= tClosestPoint.z;
					}

					tAABBClosest = XMLoadFloat3(&tClosestPoint);

					DirectX::XMStoreFloat3(&_tOffset, tAABBClosest);

				}
				else
				{
					DirectX::XMStoreFloat3(&_tOffset, tDistanceResults / fVectorModifier);
				}
			}
			//Else, find the remainder radius and get the modifier by dividing the magnitude and the remainder radius
			//then divided it by the distance to get the offset.
			else
			{
				float fRemainderRadius = tOtherSphere.m_fRadius - tMagnitude.x;

				float fVectorModifier = tMagnitude.x / fRemainderRadius;

				DirectX::XMStoreFloat3(&_tOffset, -tDistanceResults / fVectorModifier);
			}
			break;
		}
		case CAPSULE:
		{
			//Find the Closest Point of an AABB.
			CCapsuleCollider* cOtherCollider = ((CCapsuleCollider*)(_other));

			//Get AABB CenterPoint
			XMFLOAT3 tStart = cOtherCollider->GetCapsule().m_tsegment.m_tStart;
			XMFLOAT3 tEnd = cOtherCollider->GetCapsule().m_tsegment.m_tEnd;

			XMVECTOR tMin = XMLoadFloat3(&m_tCurrentAABB.m_tMin);
			XMVECTOR tMax = XMLoadFloat3(&m_tCurrentAABB.m_tMax);
			XMFLOAT3 tAABBCenterPoint;

			DirectX::XMStoreFloat3(&tAABBCenterPoint, (tMin + tMax) * 0.5f);

			//Get the AABB Half-Extents
			XMVECTOR tAABBCenter = XMLoadFloat3(&tAABBCenterPoint);
			XMFLOAT3 tAABBHalfExtent;

			DirectX::XMStoreFloat3(&tAABBHalfExtent, tMax - tAABBCenter);

			//Get segment midpoint
			XMVECTOR tStartPoint = XMLoadFloat3(&tStart);
			XMVECTOR tEndPoint = XMLoadFloat3(&tEnd);
			XMFLOAT3 tMidPoint;

			DirectX::XMStoreFloat3(&tMidPoint, (tStartPoint + tEndPoint) * 0.5f);

			//Get HalfLength
			XMVECTOR tMid = XMLoadFloat3(&tMidPoint);
			XMFLOAT3 tHalfLength;

			DirectX::XMStoreFloat3(&tHalfLength, tEndPoint - tMid);

			//Midpoint = midpoint - center
			DirectX::XMStoreFloat3(&tMidPoint, tMid - tAABBCenter);

			//Try world coordinates axes as separating axes
			float fX = 0.0f;
			float fY = 0.0f;
			float fZ = 0.0f;

			float adx = abs(tHalfLength.x);

			fX = (tAABBHalfExtent.x + adx + cOtherCollider->GetCapsule().m_fRadius) - abs(tMidPoint.x);

			float ady = abs(tHalfLength.y);

			fY = (tAABBHalfExtent.y + ady + cOtherCollider->GetCapsule().m_fRadius) - abs(tMidPoint.y);

			float adz = abs(tHalfLength.z);

			fZ = (tAABBHalfExtent.z + adz + cOtherCollider->GetCapsule().m_fRadius) - abs(tMidPoint.z);

			//If either of the Capsule's X, Y or Z's position is less than the Midpoint's X, Y or Z, Flip them so we can push in the opposite direction
			if (m_tPosition.x < tMidPoint.x)
			{
				fX = -fX;
			}
			if (m_tPosition.y < tMidPoint.y)
			{
				fY = -fY;
			}
			if (m_tPosition.z < tMidPoint.z)
			{
				fZ = -fZ;
			}

			//Set the offset using the the differences founded on each axes
			_tOffset.x = fX + 0.2f;
			_tOffset.y = fY + 0.2f;
			_tOffset.z = fZ + 0.2f;

			break;
		}
	}
}

/********************************************
* BuildAABB(): Uses the list of points passed in to create an AABB
*
* Ins: _object, _unSize
*
* Outs: None
*
* Returns: None
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
void CAABBCollider::BuildAABB(CObjectBase* _object, unsigned int _unSize)
{
	// For all the provided points, find the highest X, highest Y,
	// highest Z, lowest X, lowest Y, and lowest Z.
	// Use these values to build the AABB.

	XMFLOAT3 tMin = { FLT_MAX, FLT_MAX, FLT_MAX };// = XMLoadFloat3(&m_tCurrentAABB.m_tMin);
	XMFLOAT3 tMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };// = XMLoadFloat3(&m_tCurrentAABB.m_tMax);


	auto vertcies = _object->GetModel()->GetBindPose()->GetVertices();

	for (unsigned int i = 0; i < vertcies.size(); i++)
	{
		tMin.x = min(tMin.x, vertcies[i].position.x);
		tMin.y = min(tMin.y, vertcies[i].position.y);
		tMin.z = min(tMin.z, vertcies[i].position.z);

		tMax.x = max(tMax.x, vertcies[i].position.x);
		tMax.y = max(tMax.y, vertcies[i].position.y);
		tMax.z = max(tMax.z, vertcies[i].position.z);
	}

	//for (unsigned int i = 0; i < _unSize; i++)
	//{
	//	// Get the Highest Value
	//	if (m_tInitialAABB.m_tMax.x < _object->GetModel()->GetBindPose()->GetVertices()[i].position.x)
	//		m_tInitialAABB.m_tMax.x = _object->GetModel()->GetBindPose()->GetVertices()[i].position.x;

	//	if (m_tInitialAABB.m_tMax.y < _object->GetModel()->GetBindPose()->GetVertices()[i].position.y)
	//		m_tInitialAABB.m_tMax.y = _object->GetModel()->GetBindPose()->GetVertices()[i].position.y;

	//	if (m_tInitialAABB.m_tMax.z < _object->GetModel()->GetBindPose()->GetVertices()[i].position.z)
	//		m_tInitialAABB.m_tMax.z = _object->GetModel()->GetBindPose()->GetVertices()[i].position.z;

	//	//Get the Lowest Value
	//	if (m_tInitialAABB.m_tMin.x > _object->GetModel()->GetBindPose()->GetVertices()[i].position.x)
	//		m_tInitialAABB.m_tMin.x = _object->GetModel()->GetBindPose()->GetVertices()[i].position.x;

	//	if (m_tInitialAABB.m_tMin.y > _object->GetModel()->GetBindPose()->GetVertices()[i].position.y)
	//		m_tInitialAABB.m_tMin.y = _object->GetModel()->GetBindPose()->GetVertices()[i].position.y;
	//						   
	//	if (m_tInitialAABB.m_tMin.z > _object->GetModel()->GetBindPose()->GetVertices()[i].position.z)
	//		m_tInitialAABB.m_tMin.z = _object->GetModel()->GetBindPose()->GetVertices()[i].position.z;
	//}

	m_tInitialAABB.m_tMin = tMin;
	m_tInitialAABB.m_tMax = tMax;

	if (_object->GetRotation().y == 90.0f || _object->GetRotation().y == -90.0f)
	{
		m_tInitialAABB.m_tMin.x = tMin.z;
		m_tInitialAABB.m_tMin.z = tMin.x;

		m_tInitialAABB.m_tMax.x = tMax.z;
		m_tInitialAABB.m_tMax.z = tMax.x;
	}

	XMVECTOR v1 = XMLoadFloat3(&tMin);
	XMVECTOR v2 = XMLoadFloat3(&tMax);
	XMVECTOR objPosition = XMLoadFloat3(&_object->GetPosition());
	v1 += objPosition;
	v2 += objPosition;

	XMStoreFloat3(&m_tCurrentAABB.m_tMin, v1);
	XMStoreFloat3(&m_tCurrentAABB.m_tMax, v2);

	
}