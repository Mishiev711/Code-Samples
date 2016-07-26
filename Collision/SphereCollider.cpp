#include "../PreCompiled.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "CapsuleCollider.h"

/************************************************
* Filename:      SphereCollider.cpp
* Date:          06/08/2015
* Mod. Date:     06/20/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       To handle Sphere game object's collisions
*************************************************/

/********************************************
* CSphereCollider(): Setup the variables of the Sphere Collider class. [ Constructor ]
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
CSphereCollider::CSphereCollider()
{
	m_tCurrentSphere.m_tCenter = XMFLOAT3(0, 0, 0);
	m_tCurrentSphere.m_fRadius = 1.0f;

	m_tPosition = XMFLOAT3(0, 0, 0);
	m_bActive = true;
}

/********************************************
* CSphereCollider(): Setup the variables of the Sphere Collider class. [ Overloaded Constructor ]
*
* Ins: _cMesh, _unSize
*
* Outs: None
*
* Returns: None
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
CSphereCollider::CSphereCollider(CObjectBase* _object, unsigned int _unSize)
{
	this->BuildSphere(_object, _unSize);
	m_tPosition = XMFLOAT3(0, 0, 0);//_object->GetPosition();
}

/********************************************
* CSphereCollider(): Setup the variables of the Sphere Collider class. [ Overloaded Constructor ]
*
* Ins: _tCenter, _tRadius
*
* Outs: None
*
* Returns: None
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
CSphereCollider::CSphereCollider(XMFLOAT3 _tCenter, float _fRadius)
{
	m_tCurrentSphere.m_tCenter = _tCenter;
	m_tCurrentSphere.m_fRadius = _fRadius;
	m_tPosition = _tCenter;

	m_bActive = true;
}

/********************************************
* ~CSphereCollider(): Shutdown the class. [ Destructor ]
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
CSphereCollider::~CSphereCollider(void)
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
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
eColliderID CSphereCollider::GetType() const
{
	return SPHERE;
}

/********************************************
* GetSphere(): Returns the Collider surrounding the object.
*
* Ins: None
*
* Outs: None
*
* Returns: tSphere
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
tSphere CSphereCollider::GetSphere() const
{
	return m_tCurrentSphere;
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
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
XMFLOAT3 CSphereCollider::GetPosition() const
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
void CSphereCollider::SetPosition(XMFLOAT3 _tPosition)
{
	m_tCurrentSphere.m_tCenter.x = _tPosition.x + m_tPosition.x;
	m_tCurrentSphere.m_tCenter.y = _tPosition.y + m_tPosition.y;
	m_tCurrentSphere.m_tCenter.z = _tPosition.z + m_tPosition.z;
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
void CSphereCollider::Update()
{
	
}

/********************************************
* OnCollisionWith(): If the the Collider is not active, return false
*					 Otherwise, check the type of the other collider and perform the
*					 approperiate collision check. Then return the results.
*					 Also find out which axis did the other collider collided on.
*
* Ins: _other
*
* Outs: None
*
* Returns: bool
*
* Mod. Date 06/20/2015
* Mod. Initials: MM
********************************************/
bool CSphereCollider::CheckCollisionWith(CICollider* _other) const
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

			if (SphereToAABB(m_tCurrentSphere, cOtherCollider->GetAABB()))
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
			CSphereCollider* m_cOtherCollider = ((CSphereCollider*)(_other));

			if (SphereToSphere(m_tCurrentSphere, m_cOtherCollider->GetSphere()))
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
			CCapsuleCollider* m_cOtherCollider = ((CCapsuleCollider*)(_other));

			if (CapsuleToSphere(m_cOtherCollider->GetCapsule(), m_tCurrentSphere ))
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
void CSphereCollider::ResolveCollision(XMFLOAT3& _tOffset, CICollider* _other) const
{
	switch (_other->GetType())
	{
		case AABB:
		{
			CAABBCollider* m_pcOtherCollider = ((CAABBCollider*)(_other));

			tAABB tOtherAABB = m_pcOtherCollider->GetAABB();

			XMFLOAT3 tClosestPoint = XMFLOAT3(0, 0, 0);

			ClosestPointOnAABB(tClosestPoint, m_tCurrentSphere.m_tCenter, tOtherAABB);

			XMVECTOR tAABBClosest = XMLoadFloat3(&tClosestPoint);
			XMVECTOR tCenter = XMLoadFloat3(&m_tCurrentSphere.m_tCenter);
			XMFLOAT3 tDistance;

			DirectX::XMStoreFloat3(&tDistance, tAABBClosest - tCenter);

			XMVECTOR tDistanceResults = XMLoadFloat3(&tDistance);
			XMVECTOR tLength = XMVector3Length(tDistanceResults);
			XMFLOAT3 tMagnitude;

			DirectX::XMStoreFloat3(&tMagnitude, tLength);

			//If the Sphere is inside the AABB, Add the Magnitude to the radius and then 
			//divide the magnitude by the new radius to get the modifier and then divided the
			//distance between the Sphere's center and closest AABB point and the modifier
			if (m_tCurrentSphere.m_tCenter.x < tOtherAABB.m_tMax.x && m_tCurrentSphere.m_tCenter.x > tOtherAABB.m_tMin.x &&
				m_tCurrentSphere.m_tCenter.y < tOtherAABB.m_tMax.y && m_tCurrentSphere.m_tCenter.y > tOtherAABB.m_tMin.y &&
				m_tCurrentSphere.m_tCenter.z < tOtherAABB.m_tMax.z && m_tCurrentSphere.m_tCenter.z > tOtherAABB.m_tMin.z)
			{
				float fExtendedRadius = tMagnitude.x + m_tCurrentSphere.m_fRadius;

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
				float fRemainderRadius = m_tCurrentSphere.m_fRadius - tMagnitude.x;

				float fVectorModifier = tMagnitude.x / fRemainderRadius;

				DirectX::XMStoreFloat3(&_tOffset, -tDistanceResults / fVectorModifier);
			}

			break;
		}
		case SPHERE:
		{
			CSphereCollider* m_pcOtherCollider = ((CSphereCollider*)(_other));

			tSphere tOtherSphere = m_pcOtherCollider->GetSphere();

			XMVECTOR tCenterA = XMLoadFloat3(&m_tCurrentSphere.m_tCenter);
			XMVECTOR tCenterB = XMLoadFloat3(&tOtherSphere.m_tCenter);
			XMFLOAT3 tDistanceVector;

			DirectX::XMStoreFloat3(&tDistanceVector, tCenterB - tCenterA);

			float CombineRadius = tOtherSphere.m_fRadius + m_tCurrentSphere.m_fRadius;

			XMVECTOR tDistance = XMLoadFloat3(&tDistanceVector);
			XMVECTOR tLength = XMVector3Length(tDistance);
			XMFLOAT3 tMagnitude;

			DirectX::XMStoreFloat3(&tMagnitude, tLength);

			float MovementDistance = CombineRadius - tMagnitude.x;
			
			XMVECTOR tNormalize = DirectX::XMVector3Normalize(-tDistance);
			tNormalize *= MovementDistance;

			DirectX::XMStoreFloat3(&_tOffset, tNormalize);
			break;
		}
		case CAPSULE:
		{
			CCapsuleCollider* m_cOtherCollider = ((CCapsuleCollider*)(_other));

			//Find the Closest Point of Capsule A using the Sphere's Center.
			XMFLOAT3 tTestPoint = ClosestPointLineSegment(m_cOtherCollider->GetCapsule().m_tsegment, m_tCurrentSphere.m_tCenter);

			//Get the combined radius of the capsule and the sphere
			float fCombinedRadius = m_cOtherCollider->GetCapsule().m_fRadius + m_tCurrentSphere.m_fRadius;

			//Get the distance from the test point to the sphere center
			XMFLOAT3 tSphereCenter = m_tCurrentSphere.m_tCenter;

			XMVECTOR tCenterA = XMLoadFloat3(&tTestPoint);
			XMVECTOR tCenterB = XMLoadFloat3(&tSphereCenter);

			DirectX::XMStoreFloat3(&_tOffset, tCenterA - tCenterB);
			break;
		}
	}
}

/********************************************
* BuildSphere(): Uses the list of vertices passed in to create a Sphere and uses
*				 the position passed in to set the sphere's center.
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
void CSphereCollider::BuildSphere(CObjectBase* _object, unsigned int _unSize)
{
	m_tCurrentSphere.m_fRadius = 0.0f;
	m_tCurrentSphere.m_tCenter = XMFLOAT3(0,0,0);

	for (unsigned int i = 0; i < _unSize; i++)
	{
		XMFLOAT3 tVert = XMFLOAT3(_object->GetModel()->GetBindPose()->GetVertices()[i].position.x, _object->GetModel()->GetBindPose()->GetVertices()[i].position.y, _object->GetModel()->GetBindPose()->GetVertices()[i].position.z);
		
		XMVECTOR tPoint = XMLoadFloat3(&tVert);
		XMVECTOR tCenter = XMLoadFloat3(&m_tCurrentSphere.m_tCenter);
		XMFLOAT3 tDistance;

		DirectX::XMStoreFloat3(&tDistance, tCenter - tPoint);

		XMVECTOR tDistanceResult = XMLoadFloat3(&tDistance);
		XMVECTOR tLength = XMVector3Length(tDistanceResult);
		XMFLOAT3 tMagnitude;

		DirectX::XMStoreFloat3(&tMagnitude, tLength);

		if (tMagnitude.x > m_tCurrentSphere.m_fRadius)
		{
			m_tCurrentSphere.m_fRadius = tMagnitude.x;
		}
	}

	m_tCurrentSphere.m_fRadius *= _object->GetScale().x;

	XMVECTOR tCenter = XMLoadFloat3(&m_tCurrentSphere.m_tCenter);
	XMVECTOR tPosition = XMLoadFloat3(&_object->GetPosition());

	DirectX::XMStoreFloat3(&m_tCurrentSphere.m_tCenter, tCenter + tPosition);
}