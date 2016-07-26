#pragma once
#include <DirectXMath.h>

using namespace DirectX;

enum eColliderID {AABB, SPHERE, CAPSULE, NONE};

/************************************************
* Filename:      ICollider.h
* Date:          06/08/2015
* Mod. Date:     08/03/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       Base class for all types of colliders for game objects
*************************************************/
class CICollider
{
protected:
	XMFLOAT3 m_tPosition;
	bool m_bActive;

public:
	CICollider(void) : m_bActive(true){};
	
	virtual ~CICollider(void)
	{
		bool b = true;
		b = false;
		int i = (int)b;
		i += 100;
	}

	virtual eColliderID GetType() const { return NONE; };

	bool GetActive() const{ return m_bActive; }
	void SetActive( bool _bActive ) { this->m_bActive = _bActive; }

	virtual XMFLOAT3 GetPosition() const { return m_tPosition; }
	virtual void SetPosition(XMFLOAT3 _tPosition) { m_tPosition = _tPosition; }

	virtual void Update(){};
	virtual bool CheckCollisionWith(CICollider* _other) const { return false; };
	virtual void ResolveCollision(XMFLOAT3& _tOffset, CICollider* _other) const {};

};