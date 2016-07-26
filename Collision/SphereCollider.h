#pragma once
#include "ICollider.h"
#include "CollisionLibrary\CollisionLib.h"
#include "../AssetManager/AssetClasses/Mesh.h"
#include "../AssetManager/AssetClasses/Model.h"
#include "../ObjectManager/Headers/ObjectBase.h"
#include <DirectXMath.h>

using namespace DirectX;

/************************************************
* Filename:      SphereCollider.h
* Date:          06/08/2015
* Mod. Date:     06/19/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       To handle Sphere game object's collisions
*************************************************/

class CSphereCollider : public CICollider
{
private:
	tSphere m_tCurrentSphere;

public:
	CSphereCollider();
	CSphereCollider(XMFLOAT3 _tCenter, float _fRadius);
	CSphereCollider(CObjectBase* _object, unsigned int _unSize);
	virtual ~CSphereCollider(void);

	virtual eColliderID GetType() const;
	tSphere GetSphere() const;
	void SetSphereCenter(XMFLOAT3 _tCenter) { m_tCurrentSphere.m_tCenter = _tCenter; };
	void SetSphereRadius(float _tRadius) { m_tCurrentSphere.m_fRadius = _tRadius; };

	XMFLOAT3 GetPosition() const;
	void SetPosition(XMFLOAT3 _tPosition);

	void BuildSphere(CObjectBase* _object, unsigned int _unSize);

	void Update();
	virtual bool CheckCollisionWith(CICollider* _other) const;
	virtual void ResolveCollision(XMFLOAT3& _tOffset, CICollider* _other) const;
};