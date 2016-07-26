#include "CollisionManager.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "CapsuleCollider.h"

#include "../ObjectManager/Headers/Barrier.h"
#include "../ObjectManager/Headers/VoltDoor.h"
#include "../ObjectManager/Headers/Spawner.h"
#include "../ObjectManager/Objects/Pickup.h"
#include "../ObjectManager/Headers/Trigger.h"

/************************************************
* Filename:      CollisionManager.cpp
* Date:          07/07/2015
* Mod. Date:     07/07/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       To set objects up with the appropriate collider
*				 and to handle their collision checks and resolutions
*************************************************/

std::vector<std::list<CObjectBase*>> CCollisionManager::m_vpCollisions;

/********************************************
* CCollisionManager(): Setup the variables of the CollisionManager class. [ Constructor ]
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 07/07/2015
* Mod. Initials: MM
********************************************/
CCollisionManager::CCollisionManager()
{
	m_vpCollisions.resize(CObjectBase::OBJECT_TYPE::NUMOBJECTS + 1);
}

/********************************************
* ~CCollisionManager(): Shutdowns the CollisionManager class. [ Destructor ]
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 07/07/2015
* Mod. Initials: MM
********************************************/
CCollisionManager::~CCollisionManager()
{

}

/********************************************
* AddCollider(): Gives the appropriate collider to the object and adds it to the list.
*
* Ins: _addedObject
*
* Outs: _objectList
*
* Returns: bool
*
* Mod. Date 07/07/2015
* Mod. Initials: MM
********************************************/
void CCollisionManager::AddCollider(CObjectBase* _addedObject)
{
	//Create an ICollider
	CICollider* tTempCollider = nullptr;

	//See what type of object it is and apply the appropriate collider to the object
	switch (_addedObject->GetType())
	{
		case CObjectBase::BARRIER:
		case CObjectBase::LEVEL:
		{
			tTempCollider = new CAABBCollider(_addedObject, (unsigned int)_addedObject->GetModel()->GetBindPose()->GetVertices().size());
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::PLAYER:
		{
			tTempCollider = new CSphereCollider(XMFLOAT3(0.f, 0.f, 0.f), 1.5f);
			//tTempCollider = new CCapsuleCollider(XMFLOAT3(0, 0.0f, 0), XMFLOAT3(0, 1.0f, 0), 0.5f, _addedObject->GetPosition());
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::EBULLET:
		case CObjectBase::PBULLET:
		{
			tTempCollider = new CSphereCollider(_addedObject, (unsigned int)_addedObject->GetModel()->GetBindPose()->GetVertices().size());
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::PGRENADE:
		{
			tTempCollider = new CSphereCollider(_addedObject, (unsigned int)_addedObject->GetModel()->GetBindPose()->GetVertices().size());
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::EXPLOSION:
		{
			tTempCollider = new CSphereCollider(_addedObject, (unsigned int)_addedObject->GetModel()->GetBindPose()->GetVertices().size());
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::SPAWNER:
		{
			tTempCollider = new CSphereCollider(_addedObject, (unsigned int)_addedObject->GetModel()->GetBindPose()->GetVertices().size());
			//tTempCollider = new CCapsuleCollider(_addedObject, (unsigned int)_addedObject->GetModel()->GetBindPose()->GetVertices().size());
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::TURRET:
		{
			tTempCollider = new CSphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::MELEE:
		{
			tTempCollider = new CSphereCollider;
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::KAMAKAZI:
		{
			tTempCollider = new CSphereCollider(XMFLOAT3(0, 0.0f, 0), 1.0f);
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::PICKUP:
		{
			tTempCollider = new CSphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::TRIGGER:
		{
			tTempCollider = new CAABBCollider(_addedObject, _addedObject->GetModel()->GetBindPose()->GetVertices().size());
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::SCRIPT:
		{
			tTempCollider = new CSphereCollider(_addedObject, (unsigned int)_addedObject->GetModel()->GetBindPose()->GetVertices().size());
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::VOLTDOOR:
		{
			tTempCollider = new CAABBCollider(_addedObject, _addedObject->GetModel()->GetBindPose()->GetVertices().size());
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::BOSS:
		{
			tTempCollider = new CAABBCollider(_addedObject, _addedObject->GetModel()->GetBindPose()->GetVertices().size());
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		case CObjectBase::PERMPOWERUP:
		{
			tTempCollider = new CSphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
			_addedObject->SetCollider(tTempCollider);
			break;
		}
		default:
		{
			//Debug Log: Post Message that an unknown object was found, don't add a collider to it
			break;
		}
	}

	//Add the object to the list of collisions
	_addedObject->SetCollisionCheck(false);
	m_vpCollisions[_addedObject->GetType()].push_back(_addedObject);
}

/********************************************
* RemoveCollider(): Deletes the collider from the object and removes it from the list.
*
* Ins: _removedObject
*
* Outs: _objectList
*
* Returns: bool
*
* Mod. Date 07/07/2015
* Mod. Initials: MM
********************************************/
bool CCollisionManager::RemoveCollider(CObjectBase* _removedObject)
{
	delete _removedObject->GetCollider();
	_removedObject->SetCollider(nullptr);

	m_vpCollisions[_removedObject->GetType()].remove(_removedObject);

	return true;
}

/********************************************
* CheckCollision(): Iterates through the list of objects and check to see if a collision has occured
*					If so, resolve the collision between them.
*
* Ins: _removedObject
*
* Outs: _objectList
*
* Returns: bool
*
* Mod. Date 07/07/2015
* Mod. Initials: MM
********************************************/
void CCollisionManager::CheckCollision(float _fDeltaTime)
{
	////Create a nested for loop to iterate through all types of game objects for collision checks
	for (unsigned int i = 1; i < CObjectBase::NUMOBJECTS; i++)
	{
		unsigned int j = i + 1;

		for (j; j < CObjectBase::NUMOBJECTS; j++)
		{
			//Make two iterators for the list of objects. One using 'i' as an index, the othe using 'j' as an index
			auto iter1 = m_vpCollisions[i].begin();
			auto iter2 = m_vpCollisions[j].begin();

			//While it's not the end of the list using 'i' as an index AND
			//While it's not the end of the list using 'j' as an index, check collision between both iterators
			while (iter1 != m_vpCollisions[i].end())
			{
				while (iter2 != m_vpCollisions[j].end())
				{
					//If the both buckets are the same, move on to the next object type
					if (i == j)
					{
						break;
					}

					//If the first iterator is not the same as the second iterator, continue collision check
					if (iter1._Ptr->_Myval != iter2._Ptr->_Myval)
					{
						//Create an offset variable for collision resolution
						XMFLOAT3 tOffsetA = XMFLOAT3(0, 0, 0);
						XMFLOAT3 tOffsetB = XMFLOAT3(0, 0, 0);

						//if the first or second iterator doesn't have a collider, move on the the next object
						if (!iter1._Ptr->_Myval->GetCollider() || !iter2._Ptr->_Myval->GetCollider())
						{
							iter2++;
							continue;
						}

						//If the two objects had collided, resolve the collision
						if (iter1._Ptr->_Myval->GetCollider()->CheckCollisionWith(iter2._Ptr->_Myval->GetCollider()))
						{
							//Get the collision offset for both objects
							iter1._Ptr->_Myval->GetCollider()->ResolveCollision(tOffsetA, iter2._Ptr->_Myval->GetCollider());
							iter2._Ptr->_Myval->GetCollider()->ResolveCollision(tOffsetB, iter1._Ptr->_Myval->GetCollider());

							//Combine the offsets together
							XMVECTOR tA = XMLoadFloat3(&tOffsetA);
							XMVECTOR tB = XMLoadFloat3(&tOffsetB);
							XMFLOAT3 tTotalOffset = XMFLOAT3(0, 0, 0);

							DirectX::XMStoreFloat3(&tTotalOffset, tA + tB);

							//Resolve the collision
							iter1._Ptr->_Myval->CollisionReactions(iter2._Ptr->_Myval, tTotalOffset, _fDeltaTime);
							iter2._Ptr->_Myval->CollisionReactions(iter1._Ptr->_Myval, tTotalOffset, _fDeltaTime);
						}
					}

					//Move to the next object
					iter2++;
				}

				//If the first iterator is not at the end of the list, move to the next object
				if (iter1 != m_vpCollisions[i].end())
				{
					iter1++;
				}

				//Reset the second iterator to the beginning of the list
				iter2 = m_vpCollisions[j].begin();
			}
		}
	}
}

bool CCollisionManager::LineCast(const XMFLOAT3& _tpoint1, const XMFLOAT3& _tpoint2, unsigned int bucket, XMFLOAT3& _tReturnPt)
{
	//Setup the ray struct using _tpoint1 and _tpoint2
	tRay tInitialRay;

	tInitialRay.m_tStartPoint = _tpoint1;

	XMVECTOR tPoint1 = XMLoadFloat3(&_tpoint1);
	XMVECTOR tPoint2 = XMLoadFloat3(&_tpoint2);

	DirectX::XMStoreFloat3(&tInitialRay.m_tRayDirection, tPoint2 - tPoint1);

	//Now go through the list using the bucket passed in and perform collision checks
	auto iter = m_vpCollisions[bucket].begin();

	float closestDist = 1.0f;
	while (iter != m_vpCollisions[bucket].end())
	{
		//Create an offset variable for collision resolution
		XMFLOAT3 tOffset = XMFLOAT3(0, 0, 0);

		//if the first or second iterator doesn't have a collider, move on the the next object
		if (!iter._Ptr->_Myval->GetCollider())
		{
			iter++;
			continue;
		}

		if (CObjectBase::BARRIER == bucket)
		{
			CObjectBase * temp = iter._Ptr->_Myval;
			if (!((CBarrier *)temp)->IsCollideable())
			{
				iter++;
				continue;
			}
		}

		if (CObjectBase::VOLTDOOR == bucket)
		{
			CObjectBase * temp = iter._Ptr->_Myval;
			if (!((CVoltDoor *)temp)->IsCollideable())
			{
				iter++;
				continue;
			}
		}

		//Perform the appropriate RayTo check by finding out what kind of collider the object has
		switch (iter._Ptr->_Myval->GetCollider()->GetType())
		{
			case AABB:
			{
				CAABBCollider* cOtherCollider = ((CAABBCollider*)(iter._Ptr->_Myval->GetCollider()));

				float dist;
			
				if (SegmentToAABB(cOtherCollider->GetAABB(), _tpoint1, _tpoint2, dist, closestDist))
					closestDist = dist;

				break;
			}
		}
		//If the first iterator is not at the end of the list, move to the next object
		if (iter != m_vpCollisions[bucket].end())
		{
			iter++;
		}
	}
	if (closestDist < 1.0f)
	{
		XMVECTOR dir = tPoint2 - tPoint1;

		tPoint1 = tPoint1 + (dir * closestDist);

		XMStoreFloat3(&_tReturnPt, tPoint1);

		return true;
	}

	return false;
}

XMFLOAT3 CCollisionManager::RayCast(unsigned int bucket, XMFLOAT3 _tpoint1, XMFLOAT3 _tdirection)
{
	//Setup the ray struct using _tpoint1 and _tpoint2
	tRay tInitialRay;

	tInitialRay.m_tStartPoint = _tpoint1;
	tInitialRay.m_tRayDirection = _tdirection;

	//Now go through the list using the bucket passed in and perform collision checks
	auto iter = m_vpCollisions[bucket].begin();

	while (iter != m_vpCollisions[bucket].end())
	{
		//if the first or second iterator doesn't have a collider, move on the the next object
		if (!iter._Ptr->_Myval->GetCollider())
		{
			iter++;
			continue;
		}

		if (CObjectBase::SPAWNER == bucket)
		{
			CObjectBase * temp = iter._Ptr->_Myval;
			if (((CSpawner *)temp)->IsDead())
			{
				iter++;
				continue;
			}
		}

		if (CObjectBase::PICKUP == bucket)
		{
			CObjectBase * temp = iter._Ptr->_Myval;
			if (!((CPickup *)temp)->IsDead())
			{
				iter++;
				continue;
			}
		}

		if (CObjectBase::TRIGGER == bucket)
		{
			CObjectBase * temp = iter._Ptr->_Myval;
			if (!((CTrigger *)temp)->IsInvisible())
			{
				iter++;
				continue;
			}
		}

		//Perform the appropriate RayTo check by finding out what kind of collider the object has
		switch (iter._Ptr->_Myval->GetCollider()->GetType())
		{
			case AABB:
			{
				CAABBCollider* cOtherCollider = ((CAABBCollider*)(iter._Ptr->_Myval->GetCollider()));

				if (RayToAABB(tInitialRay, cOtherCollider->GetAABB()))
				{
					//Setup the functions min and max
					float fmin = 0.0f;
					float fmax = FLT_MAX;

					//Store all variables passed in into float arrays for the for loop
					float fMax[3], fMin[3], fDirection[3], fStartPoint[3];

					//Max setup
					fMax[0] = cOtherCollider->GetAABB().m_tMax.x;
					fMax[1] = cOtherCollider->GetAABB().m_tMax.y;
					fMax[2] = cOtherCollider->GetAABB().m_tMax.z;

					//Min setup
					fMin[0] = cOtherCollider->GetAABB().m_tMin.x;
					fMin[1] = cOtherCollider->GetAABB().m_tMin.y;
					fMin[2] = cOtherCollider->GetAABB().m_tMin.z;

					//Direction setup
					fDirection[0] = tInitialRay.m_tRayDirection.x;
					fDirection[1] = tInitialRay.m_tRayDirection.y;
					fDirection[2] = tInitialRay.m_tRayDirection.z;

					//Startpoint setup
					fStartPoint[0] = tInitialRay.m_tStartPoint.x;
					fStartPoint[1] = tInitialRay.m_tStartPoint.y;
					fStartPoint[2] = tInitialRay.m_tStartPoint.z;

					//Now check on all 3 axises for collision
					for (unsigned int i = 0; i < 3; i++)
					{
						//if the absolute value of a direction is less than the float_epsilon,
						if (abs(fDirection[i]) < FLT_EPSILON)
						{
							//the ray is parallel to the axis, return false
							if (fStartPoint[i] < fMin[i] || fStartPoint[i] > fMax[i])
							{
								return XMFLOAT3(0, 0, 0);
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
								return XMFLOAT3(0, 0, 0);
							}
						}
					}

					//If it made it to this point, the Ray has collided on all Axises.
					//Now calculate the collisionpoint and return it.
					XMFLOAT3 tCollisionPoint = XMFLOAT3(0, 0, 0);
					XMVECTOR tStartPoint = XMLoadFloat3(&tInitialRay.m_tStartPoint);
					XMVECTOR tDirection = XMLoadFloat3(&tInitialRay.m_tRayDirection);
					
					DirectX::XMStoreFloat3(&tCollisionPoint, tStartPoint + tDirection * fmin);

					return tCollisionPoint;
				}

				break;
			}
			case SPHERE:
			{
				CSphereCollider* cOtherCollider = ((CSphereCollider*)(iter._Ptr->_Myval->GetCollider()));

				//If they had collided, resolve the collision
				if (RayToSphere(tInitialRay, cOtherCollider->GetSphere()))
				{
					//Find the distance between the closest point and the sphere's center
					XMVECTOR tRayStartPoint = XMLoadFloat3(&tInitialRay.m_tStartPoint);
					XMFLOAT3 tCenter = cOtherCollider->GetSphere().m_tCenter;
					XMVECTOR tSphereCenter = XMLoadFloat3(&tCenter);
					XMFLOAT3 tLengthVector;

					DirectX::XMStoreFloat3(&tLengthVector, tSphereCenter - tRayStartPoint);

					//Now calculate the dot between the distance and the direction
					XMVECTOR tDirection = XMLoadFloat3(&tInitialRay.m_tRayDirection);
					XMVECTOR tDistance = XMLoadFloat3(&tLengthVector);
					XMVECTOR tDirectDot = XMVector3Dot(tDistance, tDirection);
					XMFLOAT3 tDirectDotResult;

					DirectX::XMStoreFloat3(&tDirectDotResult, tDirectDot);

					//Now calculate the dot of the distance by itself
					XMVECTOR tDistDot = XMVector3Dot(tDistance, tDistance);
					XMFLOAT3 tDistDotResult;

					DirectX::XMStoreFloat3(&tDistDotResult, tDistDot);

					//Now find 'c' by using the distance dot result -  the squared radius
					float fSquaredRadius = cOtherCollider->GetSphere().m_fRadius * cOtherCollider->GetSphere().m_fRadius;
					float c = tDistDotResult.x - fSquaredRadius;

					//If the the ray's startpoint is outside of the sphere AND if the ray is pointing away from the sphere, return false
					if (c > 0.0f && tDirectDotResult.x > 0.0f)
					{
						return XMFLOAT3(0, 0, 0);
					}

					//Now calculate the discriminant, and see if its negative, if so return false
					float fDiscriminant = tDirectDotResult.x * tDirectDotResult.x - c;

					if (fDiscriminant < 0.0f)
					{
						return XMFLOAT3(0, 0, 0);
					}

					//Now compute the smallest 't' value of intersection
					float t = -tDirectDotResult.x - pow(fDiscriminant, 0.5f);

					//If 't' is negative, the ray starts inside the sphere, so clamp 't' to zero
					if (t < 0.0f)
					{
						t = 0.0f;
					}

					//Now calculate the collision point and return it
					XMFLOAT3 tCollisionPoint;

					DirectX::XMStoreFloat3(&tCollisionPoint, tRayStartPoint + (t * tDirection));

					return tCollisionPoint;
				}

				break;
			}
		}

		//If the first iterator is not at the end of the list, move to the next object
		if (iter != m_vpCollisions[bucket].end())
		{
			iter++;
		}
	}

	return XMFLOAT3(0, 0, 0);
}

void CCollisionManager::ClearColliders()
{
	for (unsigned int i = 0; i < m_vpCollisions.size(); i++)
	{
		unsigned int size = m_vpCollisions[i].size();

		for (unsigned int j = 0; j < size; j++)
		{
			delete m_vpCollisions[i].front()->GetCollider();
			m_vpCollisions[i].front()->SetCollider(nullptr);
			m_vpCollisions[i].remove(m_vpCollisions[i].front());
		}
	}
}