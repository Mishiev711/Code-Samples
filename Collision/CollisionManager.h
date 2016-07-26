#pragma once
#include "../PreCompiled.h"
#include "ICollider.h"
#include <DirectXMath.h>
#include <vector>
#include "CollisionLibrary\CollisionLib.h"
#include "../ObjectManager/Headers/ObjectBase.h"
/************************************************
* Filename:      CollisionManager.h
* Date:          07/07/2015
* Mod. Date:     07/07/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       To set objects up with the appropriate collider
*				 and to handle their collision checks and resolutions
*************************************************/

class CCollisionManager
{
private:
	static std::vector<std::list<CObjectBase*>> m_vpCollisions;

public:
	CCollisionManager();
	~CCollisionManager();

	void AddCollider(CObjectBase* _addedObject);
	bool RemoveCollider(CObjectBase* _removedObject);
	void CheckCollision(float _fDeltaTime);
	void ClearColliders();
	//ONLY WORKS WITH AABBS AT THE MOMENT
	static bool LineCast(const XMFLOAT3& _tpoint1, const XMFLOAT3& _tpoint2, unsigned int bucket, XMFLOAT3& _tReturnPt);

	static XMFLOAT3 RayCast(XMFLOAT3 _tpoint1, XMFLOAT3 _tpoint2, unsigned int bucket);
	static XMFLOAT3 RayCast(unsigned int bucket, XMFLOAT3 _tpoint1, XMFLOAT3 _tdirection);
};

