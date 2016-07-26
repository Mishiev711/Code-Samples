#pragma once
#include "ICollider.h"
#include <DirectXMath.h>
#include "CollisionLibrary\CollisionLib.h"
#include "../AssetManager/AssetClasses/Mesh.h"
#include "../AssetManager/AssetClasses/Model.h"
#include "../ObjectManager/Headers/ObjectBase.h"

using namespace DirectX;

/************************************************
* Filename:      AABBCollider.h
* Date:          06/08/2015
* Mod. Date:     06/20/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       To handle AABB game object's collisions
*************************************************/

class CAABBCollider : public CICollider
{
private:
	tAABB m_tInitialAABB;
	tAABB m_tCurrentAABB;

public:
	CAABBCollider();
	CAABBCollider(CObjectBase* _object, unsigned int _unSize);
	virtual ~CAABBCollider(void);

	virtual eColliderID GetType() const;
	tAABB GetAABB() const;

	XMFLOAT3 GetPosition() const;
	void SetPosition(XMFLOAT3 _tPosition);

	void BuildAABB(CObjectBase* _object, unsigned int _unSize);

	void Update();
	virtual bool CheckCollisionWith(CICollider* _other) const;
	virtual void ResolveCollision(XMFLOAT3& _tOffset, CICollider* _other) const;

};

