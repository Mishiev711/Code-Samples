#include "ObjectPoolSystem.h"
#include "Headers/EnemyObject.h"
#include "Headers/Bullet.h"
#include "Objects/Grenade.h"
#include "Headers/Spawner.h"
#include "Headers/Barrier.h"
#include "Headers\Trigger.h"
#include "Headers\PlayerObject.h"
#include "Headers\CodedScript.h"
#include "Objects\Explosion.h"
#include "Objects\Pickup.h"
#include "../AssetManager/AssetClasses/Model.h"
#include "../AI/Headers/AIManager.h"
#include "../AI/Headers/AIAgentBase.h"
#include "../AI/Headers/TurretAgent.h"
#include "../MessageManager/EventSystem.h"
#include "PermPowerUp.h"
#include "../Renderer/PointLight.h"
#include "../Renderer/cRenderer.h"
#include "../Renderer/LightingEngine.h"

/************************************************
* Filename:      ObjectPoolSystem.cpp
* Date:          07/22/2015
* Mod. Date:     07/22/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       To create several pools of objects
*				 for an alive and dead list.
*************************************************/

#define MAX_ENEMIES 150
#define MAX_BULLETS 5000
#define MAX_GRENADES 500
#define MAX_SPAWNERS 500
#define MAX_TRIGGERS 50
#define MAX_BARRIERS 25
#define MAX_SCRIPTS 20
#define MAX_EXPLOSION 50
#define MAX_PICKUPS 50
#define MAX_WAVES 20
#define MAX_POWERUPS 20

/********************************************
* CObjectPoolSystem(): Setup the variables of the ObjectPoolSystem class. [ Constructor ]
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
CObjectPoolSystem::CObjectPoolSystem()
{

}

/********************************************
* ~CObjectPoolSystem(): Shutdown the ObjectPoolSystem class. [ Destructor ]
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
CObjectPoolSystem::~CObjectPoolSystem()
{
	
}

/********************************************
* Initialize(): Initialize the ObjectPoolSystem class.
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::Initialize(cRenderer* renderer)
{
	//Initialize all the pools of objects
	m_vpBulletPool.resize(MAX_BULLETS);
	m_vpGrenadePool.resize(MAX_GRENADES);
	m_vpEnemyPool.resize(MAX_ENEMIES);
	m_vpSpawnerPool.resize(MAX_SPAWNERS);
	m_vpTriggerPool.resize(MAX_TRIGGERS);
	m_vpBarrierPool.resize(MAX_BARRIERS);
	m_vpCodedScriptPool.resize(MAX_SCRIPTS);
	m_vpExplosionPool.resize(MAX_EXPLOSION);
	m_vpPickupPool.resize(MAX_PICKUPS);
	m_vpPermPowerUp.resize(MAX_POWERUPS);

	m_vpDeadBullets.resize(MAX_BULLETS);
	m_vpDeadGrenades.resize(MAX_GRENADES);
	m_vpDeadEnemies.resize(MAX_ENEMIES);
	m_vpDeadSpawners.resize(MAX_SPAWNERS);
	m_vpDeadTriggers.resize(MAX_TRIGGERS);
	m_vpDeadBarriers.resize(MAX_BARRIERS);
	m_vpDeadScripts.resize(MAX_SCRIPTS);
	m_vpDeadExplosion.resize(MAX_EXPLOSION);
	m_vpDeadPickup.resize(MAX_PICKUPS);
	m_vpDeadPermPowerUp.resize(MAX_POWERUPS);

	this->renderer = renderer;

	//Create a pool of bullets
	for (unsigned int i = 0; i < MAX_BULLETS; i++)
	{
		PointLight* light = new PointLight;
		memset(light, 0, sizeof(PointLight));
		m_vpBulletPool[i] = new CBullet(light);

		m_vpDeadBullets[i] = m_vpBulletPool[i];
		renderer->GetLighting()->AddPointLight(light);
	}

	//Create a pool of Grenades
	for (unsigned int i = 0; i < MAX_GRENADES; i++)
	{
		PointLight* light = new PointLight;
		memset(light, 0, sizeof(PointLight));
		m_vpGrenadePool[i] = new CGrenade(light);
		m_vpDeadGrenades[i] = m_vpGrenadePool[i];

		renderer->GetLighting()->AddPointLight(light);
	}

	//Create a pool of Enemies
	for (unsigned int i = 0; i < MAX_ENEMIES; i++)
	{
		m_vpEnemyPool[i] = new CEnemyObject();
		m_vpDeadEnemies[i] = m_vpEnemyPool[i];
	}

	//Create a pool of Spawners
	for (unsigned int i = 0; i < MAX_SPAWNERS; i++)
	{
		m_vpSpawnerPool[i] = new CSpawner();
		m_vpDeadSpawners[i] = m_vpSpawnerPool[i];
	}

	//Create a pool of Triggers
	for (unsigned int i = 0; i < MAX_TRIGGERS; i++)
	{
		m_vpTriggerPool[i] = new CTrigger();
		m_vpDeadTriggers[i] = m_vpTriggerPool[i];
	}

	//Create a pool of Barriers
	for (unsigned int i = 0; i < MAX_BARRIERS; i++)
	{
		m_vpBarrierPool[i] = new CBarrier();
		m_vpDeadBarriers[i] = m_vpBarrierPool[i];
	}

	//Create a pool of CodedScripts
	for (unsigned int i = 0; i < MAX_SCRIPTS; i++)
	{
		m_vpCodedScriptPool[i] = new CCodedScript();
		m_vpDeadScripts[i] = m_vpCodedScriptPool[i];
	}

	//Create a pool of Explosion
	for (unsigned int i = 0; i < MAX_EXPLOSION; i++)
	{
		m_vpExplosionPool[i] = new CExplosion();
		m_vpDeadExplosion[i] = m_vpExplosionPool[i];
	}

	//Create a pool of Pickups
	for (unsigned int i = 0; i < MAX_PICKUPS; i++)
	{
		m_vpPickupPool[i] = new CPickup();
		m_vpDeadPickup[i] = m_vpPickupPool[i];
	}

	//Create a pool of Power Ups
	for (unsigned int i = 0; i < MAX_POWERUPS; i++)
	{
		m_vpPermPowerUp[i] = new CPermPowerUp();
		m_vpDeadPermPowerUp[i] = m_vpPermPowerUp[i];
	}
}

/********************************************
* Shutdown(): Shutdowns the ObjectPoolSystem class.
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::Shutdown()
{
	//Delete all Bullets
	for (unsigned int i = 0; i < MAX_BULLETS; i++)
	{
		delete m_vpBulletPool[i];
	}

	//Delete all Grenades
	for (unsigned int i = 0; i < MAX_GRENADES; i++)
	{
		delete m_vpGrenadePool[i];
	}

	//Delete all Enemies
	for (unsigned int i = 0; i < MAX_ENEMIES; i++)
	{
		delete m_vpEnemyPool[i];
	}

	//Delete all Spawners
	for (unsigned int i = 0; i < MAX_SPAWNERS; i++)
	{
		delete m_vpSpawnerPool[i];
	}

	//Delete all Triggers
	for (unsigned int i = 0; i < MAX_TRIGGERS; i++)
	{
		delete m_vpTriggerPool[i];
	}

	//Delete all Barriers
	for (unsigned int i = 0; i < MAX_BARRIERS; i++)
	{
		delete m_vpBarrierPool[i];
	}

	//Delete all CodedScripts
	for (unsigned int i = 0; i < MAX_SCRIPTS; i++)
	{
		delete m_vpCodedScriptPool[i];
	}

	//Delete all Explosion
	for (unsigned int i = 0; i < MAX_EXPLOSION; i++)
	{
		delete m_vpExplosionPool[i];
	}

	//Delete all Pickups
	for (unsigned int i = 0; i < MAX_PICKUPS; i++)
	{
		delete m_vpPickupPool[i];
	}

	//Delete all PowerUps
	for (unsigned int i = 0; i < MAX_POWERUPS; i++)
	{
		delete m_vpPermPowerUp[i];
	}
}

/********************************************
* ClearObjectPools(): Clears all the objects in the pools and resets the dead pointer to the memory
*
* Ins: None
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::ClearObjectPools()
{
	//Clear out all of the Alive Pools
	if (m_vpAliveTriggers.size() != 0)
	{
		m_vpAliveTriggers.erase(m_vpAliveTriggers.begin(), m_vpAliveTriggers.end());
	}

	if (m_vpAliveSpawners.size() != 0)
	{
		m_vpAliveSpawners.erase(m_vpAliveSpawners.begin(), m_vpAliveSpawners.end());
	}

	if (m_vpAliveBullets.size() != 0)
	{
		m_vpAliveBullets.erase(m_vpAliveBullets.begin(), m_vpAliveBullets.end());
	}

	if (m_vpAliveGrenades.size() != 0)
	{
		m_vpAliveGrenades.erase(m_vpAliveGrenades.begin(), m_vpAliveGrenades.end());
	}

	if (m_vpAliveEnemies.size() != 0)
	{
		m_vpAliveEnemies.erase(m_vpAliveEnemies.begin(), m_vpAliveEnemies.end());
	}

	if (m_vpAliveBarriers.size() != 0)
	{
		m_vpAliveBarriers.erase(m_vpAliveBarriers.begin(), m_vpAliveBarriers.end());
	}

	if (m_vpAliveScripts.size() != 0)
	{
		m_vpAliveScripts.erase(m_vpAliveScripts.begin(), m_vpAliveScripts.end());
	}

	if (m_vpAliveExplosion.size() != 0)
	{
		m_vpAliveExplosion.erase(m_vpAliveExplosion.begin(), m_vpAliveExplosion.end());
	}

	if (m_vpAlivePickup.size() != 0)
	{
		m_vpAlivePickup.erase(m_vpAlivePickup.begin(), m_vpAlivePickup.end());
	}

	if (m_vpAlivePermPowerUp.size() != 0)
	{
		m_vpAlivePermPowerUp.erase(m_vpAlivePermPowerUp.begin(), m_vpAlivePermPowerUp.end());
	}

	//Clear out the Dead Pools
	m_vpDeadTriggers.erase(m_vpDeadTriggers.begin(), m_vpDeadTriggers.end());
	m_vpDeadSpawners.erase(m_vpDeadSpawners.begin(), m_vpDeadSpawners.end());
	m_vpDeadBullets.erase(m_vpDeadBullets.begin(), m_vpDeadBullets.end());
	m_vpDeadGrenades.erase(m_vpDeadGrenades.begin(), m_vpDeadGrenades.end());
	m_vpDeadEnemies.erase(m_vpDeadEnemies.begin(), m_vpDeadEnemies.end());
	m_vpDeadBarriers.erase(m_vpDeadBarriers.begin(), m_vpDeadBarriers.end());
	m_vpDeadScripts.erase(m_vpDeadScripts.begin(), m_vpDeadScripts.end());
	m_vpDeadExplosion.erase(m_vpDeadExplosion.begin(), m_vpDeadExplosion.end());
	m_vpDeadPickup.erase(m_vpDeadPickup.begin(), m_vpDeadPickup.end());
	m_vpDeadPermPowerUp.erase(m_vpDeadPermPowerUp.begin(), m_vpDeadPermPowerUp.end());

	m_vpDeadBullets.resize(MAX_BULLETS);
	m_vpDeadGrenades.resize(MAX_GRENADES);
	m_vpDeadEnemies.resize(MAX_ENEMIES);
	m_vpDeadSpawners.resize(MAX_SPAWNERS);
	m_vpDeadTriggers.resize(MAX_TRIGGERS);
	m_vpDeadBarriers.resize(MAX_BARRIERS);
	m_vpDeadScripts.resize(MAX_SCRIPTS);
	m_vpDeadExplosion.resize(MAX_EXPLOSION);
	m_vpDeadPickup.resize(MAX_PICKUPS);
	m_vpDeadPermPowerUp.resize(MAX_POWERUPS);

	//Reset the Dead Pools to the ObjectPools
	for (unsigned int i = 0; i < MAX_BULLETS; i++)
	{
		m_vpDeadBullets[i] = m_vpBulletPool[i];
	}

	//Reset a pool of Grenades
	for (unsigned int i = 0; i < MAX_GRENADES; i++)
	{
		m_vpDeadGrenades[i] = m_vpGrenadePool[i];
	}

	//Reset a pool of Enemies
	for (unsigned int i = 0; i < MAX_ENEMIES; i++)
	{
		m_vpDeadEnemies[i] = m_vpEnemyPool[i];
	}

	//Reset a pool of Spawners
	for (unsigned int i = 0; i < MAX_SPAWNERS; i++)
	{
		m_vpDeadSpawners[i] = m_vpSpawnerPool[i];
	}

	//Reset a pool of Triggers
	for (unsigned int i = 0; i < MAX_TRIGGERS; i++)
	{
		m_vpDeadTriggers[i] = m_vpTriggerPool[i];
	}

	//Reset a pool of Barriers
	for (unsigned int i = 0; i < MAX_BARRIERS; i++)
	{
		m_vpDeadBarriers[i] = m_vpBarrierPool[i];
	}

	//Reset a pool of CodedScripts
	for (unsigned int i = 0; i < MAX_SCRIPTS; i++)
	{
		m_vpDeadScripts[i] = m_vpCodedScriptPool[i];
	}

	//Create a pool of Explosion
	for (unsigned int i = 0; i < MAX_EXPLOSION; i++)
	{
		m_vpDeadExplosion[i] = m_vpExplosionPool[i];
	}

	//Create a pool of Pickups
	for (unsigned int i = 0; i < MAX_PICKUPS; i++)
	{
		m_vpDeadPickup[i] = m_vpPickupPool[i];
	}

	//Create a pool of Power ups
	for (unsigned int i = 0; i < MAX_POWERUPS; i++)
	{
		m_vpDeadPermPowerUp[i] = m_vpPermPowerUp[i];
	}
}

/********************************************
* AddBullet(): Checks the dead list for an inactive bullet, sets it to active
*			   and adds it to the alive list. Then removes it from the dead list.
*
* Ins: _fDamage, _nBulletType, _d3dPosition, _d3dDirection, _bSpecial, _cAsset, _cAudio
*
* Outs: None
*
* Returns: CBullet*
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
CBullet* CObjectPoolSystem::AddBullet(float _fDamage, int _nBulletType, DirectX::XMFLOAT3 _d3dPosition, DirectX::XMFLOAT3 _d3dDirection, bool _bSpecial, AssetManager* _cAsset, AudioSystemWwise* _cAudio)
{
	//Find a dead bullet to set alive
	for (unsigned int i = 0; i < m_vpDeadBullets.size(); i++)
	{
		if (!m_vpDeadBullets[i]->GetActive())
		{
			if (_nBulletType == 0)
			{
				//Setup the bullet using the parameters passed in
				m_vpDeadBullets[i]->Create(_cAsset, _d3dPosition, _d3dDirection, CBullet::EBulletType::MG, CObjectBase::PBULLET, _bSpecial);
				m_vpDeadBullets[i]->SetActive(true);
				m_vpDeadBullets[i]->SetType(CObjectBase::PBULLET);
				m_vpDeadBullets[i]->SetAudio(_cAudio);
				m_vpDeadBullets[i]->SetDamage((int)_fDamage);

				//Add it to the alive list and remove it from dead list
				CBullet* tempBullet = m_vpDeadBullets[i];

				m_vpAliveBullets.push_back(m_vpDeadBullets[i]);
				m_vpDeadBullets.erase(m_vpDeadBullets.begin() + i);

				return tempBullet;
			}
			else if (_nBulletType == 1)
			{
				DirectX::XMVECTOR randomDir = XMLoadFloat3(&_d3dDirection);
				DirectX::XMVECTOR axis = XMLoadFloat3(&DirectX::XMFLOAT3(0, 1, 0));

				// ROTATE Y
				float fAmount = (((float)(rand() % 20)) / 200.0f) - 0.05f;
				axis = DirectX::XMQuaternionRotationAxis(axis, fAmount);
				randomDir = XMVector3Rotate(randomDir, axis);

				//ROTATE X
				axis = XMLoadFloat3(&DirectX::XMFLOAT3(1, 0, 0));

				fAmount = (((float)(rand() % 20)) / 200.0f) - 0.05f;
				axis = DirectX::XMQuaternionRotationAxis(axis, fAmount);
				randomDir = XMVector3Rotate(randomDir, axis);

				// APPLY
				DirectX::XMFLOAT3 newDir;
				XMStoreFloat3(&newDir, randomDir);

				//Setup the bullet using the parameters passed in
				m_vpDeadBullets[i]->Create(_cAsset, _d3dPosition, newDir, CBullet::EBulletType::SG, CObjectBase::PBULLET, _bSpecial);
				m_vpDeadBullets[i]->SetActive(true);
				m_vpDeadBullets[i]->SetType(CObjectBase::PBULLET);
				m_vpDeadBullets[i]->SetAudio(_cAudio);
				m_vpDeadBullets[i]->SetDamage((int)_fDamage);

				//Add it to the alive list and remove it from dead list
				CBullet* tempBullet = m_vpDeadBullets[i];

				m_vpAliveBullets.push_back(m_vpDeadBullets[i]);
				m_vpDeadBullets.erase(m_vpDeadBullets.begin() + i);

				return tempBullet;
			}
			else if (_nBulletType == 2)
			{
				//Setup the bullet using the parameters passed in
				m_vpDeadBullets[i]->Create(_cAsset, _d3dPosition, _d3dDirection, CBullet::EBulletType::MG, CObjectBase::EBULLET, _bSpecial);
				m_vpDeadBullets[i]->SetActive(true);
				m_vpDeadBullets[i]->SetType(CObjectBase::EBULLET);
				m_vpDeadBullets[i]->SetAudio(_cAudio);
				m_vpDeadBullets[i]->SetDamage((int)_fDamage);

				//Add it to the alive list and remove it from dead list
				CBullet* tempBullet = m_vpDeadBullets[i];

				m_vpAliveBullets.push_back(m_vpDeadBullets[i]);
				m_vpDeadBullets.erase(m_vpDeadBullets.begin() + i);

				return tempBullet;
			}
			else if (_nBulletType == 3)
			{
				//Setup the bullet using the parameters passed in
				m_vpDeadBullets[i]->Create(_cAsset, _d3dPosition, _d3dDirection, CBullet::EBulletType::LG, CObjectBase::PBULLET, _bSpecial);
				m_vpDeadBullets[i]->SetActive(true);
				m_vpDeadBullets[i]->SetType(CObjectBase::PBULLET);
				m_vpDeadBullets[i]->SetAudio(_cAudio);
				m_vpDeadBullets[i]->SetDamage((int)_fDamage);

				//Add it to the alive list and remove it from dead list
				CBullet* tempBullet = m_vpDeadBullets[i];

				m_vpAliveBullets.push_back(m_vpDeadBullets[i]);
				m_vpDeadBullets.erase(m_vpDeadBullets.begin() + i);

				return tempBullet;
			}
		}
	}

	//If something went wrong, return nullptr
	return nullptr;
}

/********************************************
* RemoveBullet(): Removes the bullet passed in from the active list
*				  and adds it to the dead list.
*
* Ins: _cRemovedBullet
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::RemoveBullet(CObjectBase* _cRemovedBullet)
{
	for (unsigned int i = 0; i < m_vpAliveBullets.size(); i++)
	{
		if (m_vpAliveBullets[i] == _cRemovedBullet)
		{
			//Set it's active variable to false
			_cRemovedBullet->SetActive(false);

			//Add it to the dead list and remove it from the alive list
			m_vpDeadBullets.push_back((CBullet*)_cRemovedBullet);
			m_vpAliveBullets.erase(m_vpAliveBullets.begin() + i);

			return;
		}
	}
}

/********************************************
* AddGrenade(): Checks the dead list for an inactive grenade, sets it to active
*			    and adds it to the alive list. Then removes it from the dead list.
*
* Ins: _fDamage, _fExplosionRadius, _d3dPosition, _d3dDirection, _bSpecial, _cAsset, _cAudio
*
* Outs: None
*
* Returns: CGrenade*
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
CGrenade* CObjectPoolSystem::AddGrenade(float _fDamage, float _fExplosionRadius, DirectX::XMFLOAT3 _d3dPosition, DirectX::XMFLOAT3 _d3dDirection, bool _bSpecial, AssetManager* _cAsset, AudioSystemWwise* _cAudio)
{
	//Find a dead grenade to set alive
	for (unsigned int i = 0; i < m_vpDeadGrenades.size(); i++)
	{
		if (!m_vpDeadGrenades[i]->GetActive())
		{
			//Setup the bullet using the parameters passed in
			m_vpDeadGrenades[i]->Create(_cAsset, _d3dPosition, _d3dDirection, CBullet::EBulletType::GL, CObjectBase::PBULLET, _bSpecial);
			m_vpDeadGrenades[i]->SetActive(true);
			m_vpDeadGrenades[i]->SetType(CObjectBase::PGRENADE);
			m_vpDeadGrenades[i]->SetAudio(_cAudio);
			m_vpDeadGrenades[i]->SetDamage((int)_fDamage);
			m_vpDeadGrenades[i]->SetExplosionRadius(_fExplosionRadius);

			//Add it to the alive list and remove it from dead list
			CGrenade* tempGrenade = m_vpDeadGrenades[i];

			m_vpAliveGrenades.push_back(m_vpDeadGrenades[i]);
			m_vpDeadGrenades.erase(m_vpDeadGrenades.begin() + i);

			return tempGrenade;
		}
	}

	//If something went wrong, return nullptr
	return nullptr;
}

/********************************************
* RemoveGrenade(): Removes the grenade passed in from the active list
*				   and adds it to the dead list.
*
* Ins: _cRemovedGrenade
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::RemoveGrenade(CObjectBase* _cRemovedGrenade)
{
	for (unsigned int i = 0; i < m_vpAliveGrenades.size(); i++)
	{
		if (m_vpAliveGrenades[i] == _cRemovedGrenade)
		{
			//Set it's active variable to false
			_cRemovedGrenade->SetActive(false);

			//Add it to the dead list and remove it from the alive list
			m_vpDeadGrenades.push_back((CGrenade*)_cRemovedGrenade);
			m_vpAliveGrenades.erase(m_vpAliveGrenades.begin() + i);

			return;
		}
	}
}

/********************************************
* AddEnemy(): Checks the dead list for an inactive enemy, sets it to active
*			  and adds it to the alive list. Then removes it from the dead list.
*
* Ins: _nEnemyType, _d3dPosition, _pcSpawner, _cAsset, _cAudio, _pcAIManager, _pcPlayer
*
* Outs: None
*
* Returns: CEnemyObject*
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
CEnemyObject* CObjectPoolSystem::AddEnemy(int _nEnemyType, DirectX::XMFLOAT3 _d3dPosition, CSpawner * _pcSpawner, AssetManager* _cAsset, AudioSystemWwise* _cAudio, CAIManager* _pcAIManager, CPlayerObject* _pcPlayer)
{
	for (unsigned int i = 0; i < m_vpDeadEnemies.size(); i++)
	{
		if (!m_vpDeadEnemies[i]->GetActive())
		{	
			m_vpDeadEnemies[i]->SetAudio(_cAudio);
			m_vpDeadEnemies[i]->Create(_cAsset, _d3dPosition, _nEnemyType, _pcSpawner);
			m_vpDeadEnemies[i]->SetActive(true);
			_pcAIManager->AddAI(m_vpDeadEnemies[i]->GetAgent());
			m_vpDeadEnemies[i]->GetAgent()->SetTarget(_pcPlayer);
			

			//Add it to the alive list and remove it from dead list
			CEnemyObject* tempEnemy = m_vpDeadEnemies[i];

			m_vpAliveEnemies.push_back(m_vpDeadEnemies[i]);
			m_vpDeadEnemies.erase(m_vpDeadEnemies.begin() + i);

			return tempEnemy;
		}
	}

	//If something went wrong, return nullptr
	return nullptr;
}

/********************************************
* RemoveEnemy(): Removes the enemy passed in from the active list
*				 and adds it to the dead list.
*
* Ins: _cRemovedEnemy
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::RemoveEnemy(CObjectBase* _cRemovedEnemy)
{
	for (unsigned int i = 0; i < m_vpAliveEnemies.size(); i++)
	{
		if (m_vpAliveEnemies[i] == _cRemovedEnemy)
		{
			//Set it's active variable to false
			_cRemovedEnemy->SetActive(false);

			//Add it to the dead list and remove it from the alive list
			m_vpDeadEnemies.push_back((CEnemyObject*)_cRemovedEnemy);
			m_vpAliveEnemies.erase(m_vpAliveEnemies.begin() + i);

			return;
		}
	}
}

/********************************************
* AddSpawner(): Checks the dead list for an inactive spawner, sets it to active
*			    and adds it to the alive list. Then removes it from the dead list.
*
* Ins: _nSpawnerType, _d3dPosition, _unWaveIndex, _cAsset, _cAudio
*
* Outs: None
*
* Returns: CSpawner*
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
CSpawner* CObjectPoolSystem::AddSpawner(int _nSpawnerType, DirectX::XMFLOAT3 _d3dPosition, bool _bHasScript, AssetManager* _cAsset, AudioSystemWwise* _cAudio)
{
	for (unsigned int i = 0; i < m_vpDeadSpawners.size(); i++)
	{
		if (!m_vpDeadSpawners[i]->GetActive())
		{
			m_vpDeadSpawners[i]->Create(_cAsset, _d3dPosition);
			m_vpDeadSpawners[i]->SetActive(true);
			m_vpDeadSpawners[i]->SetAudio(_cAudio);
			m_vpDeadSpawners[i]->SetCanDropScript(_bHasScript);
			m_vpDeadSpawners[i]->SetAlive();

			//Add it to the alive list and remove it from dead list
			CSpawner* tempSpawner = m_vpDeadSpawners[i];

			m_vpAliveSpawners.push_back(m_vpDeadSpawners[i]);
			m_vpDeadSpawners.erase(m_vpDeadSpawners.begin() + i);

			return tempSpawner;
		}
	}

	//If something went wrong, return nullptr
	return nullptr;
}

/********************************************
* RemoveSpawner(): Removes the spawner passed in from the active list
*				   and adds it to the dead list.
*
* Ins: _cRemovedSpawner
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::RemoveSpawner(CObjectBase* _cRemovedSpawner)
{
	for (unsigned int i = 0; i < m_vpAliveSpawners.size(); i++)
	{
		if (m_vpAliveSpawners[i] == _cRemovedSpawner)
		{
			//Set it's active variable to false
			_cRemovedSpawner->SetActive(false);
			//Add it to the dead list and remove it from the alive list
			m_vpDeadSpawners.push_back((CSpawner*)_cRemovedSpawner);
			m_vpAliveSpawners.erase(m_vpAliveSpawners.begin() + i);

			return;
		}
	}
}

/********************************************
* AddTrigger(): Checks the dead list for an inactive trigger, sets it to active
*			    and adds it to the alive list. Then removes it from the dead list.
*
* Ins: position, model, _cAsset, _cAudio
*
* Outs: None
*
* Returns: CTrigger*
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
CTrigger* CObjectPoolSystem::AddTrigger(DirectX::XMFLOAT3 position, CModel* model, AssetManager* _cAsset, AudioSystemWwise* _cAudio)
{
	for (unsigned int i = 0; i < m_vpDeadTriggers.size(); i++)
	{
		if (!m_vpDeadTriggers[i]->GetActive())
		{
			m_vpDeadTriggers[i]->Create(_cAsset);
			m_vpDeadTriggers[i]->SetModel(model);
			m_vpDeadTriggers[i]->SetPosition(position);
			m_vpDeadTriggers[i]->SetActive(true);

			//Add it to the alive list and remove it from dead list
			CTrigger* tempTrigger = m_vpDeadTriggers[i];

			m_vpAliveTriggers.push_back(m_vpDeadTriggers[i]);
			m_vpDeadTriggers.erase(m_vpDeadTriggers.begin() + i);

			return tempTrigger;
		}
	}

	//If something went wrong, return nullptr
	return nullptr;
}

/********************************************
* RemoveTrigger(): Removes the trigger passed in from the active list
*				   and adds it to the dead list.
*
* Ins: _cRemovedTrigger
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::RemoveTrigger(CObjectBase* _cRemovedTrigger)
{
	for (unsigned int i = 0; i < m_vpAliveTriggers.size(); i++)
	{
		if (m_vpAliveTriggers[i] == _cRemovedTrigger)
		{
			//Set it's active variable to false
			_cRemovedTrigger->SetActive(false);

			//Add it to the dead list and remove it from the alive list
			m_vpDeadTriggers.push_back((CTrigger*)_cRemovedTrigger);
			m_vpAliveTriggers.erase(m_vpAliveTriggers.begin() + i);

			return;
		}
	}
}

/********************************************
* AddBarrier(): Checks the dead list for an inactive barrier, sets it to active
*			    and adds it to the alive list. Then removes it from the dead list.
*
* Ins: position, _cAsset, _cAudio
*
* Outs: None
*
* Returns: CBarrier*
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
CBarrier* CObjectPoolSystem::AddBarrier(DirectX::XMFLOAT3 position, AssetManager* _cAsset, AudioSystemWwise* _cAudio)
{
	for (unsigned int i = 0; i < m_vpDeadBarriers.size(); i++)
	{
		if (!m_vpDeadBarriers[i]->GetActive())
		{
			m_vpDeadBarriers[i]->Create(_cAsset, position);
			m_vpDeadBarriers[i]->SetActive(true);

			//Add it to the alive list and remove it from dead list
			CBarrier* tempBarrier = m_vpDeadBarriers[i];

			m_vpAliveBarriers.push_back(m_vpDeadBarriers[i]);
			m_vpDeadBarriers.erase(m_vpDeadBarriers.begin() + i);

			return tempBarrier;
		}
	}

	//If something went wrong, return nullptr
	return nullptr;
}

/********************************************
* RemoveBarrier(): Removes the barrier passed in from the active list
*				   and adds it to the dead list.
*
* Ins: _cRemovedBarrier
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::RemoveBarrier(CObjectBase* _cRemovedBarrier)
{
	for (unsigned int i = 0; i < m_vpAliveBarriers.size(); i++)
	{
		if (m_vpAliveBarriers[i] == _cRemovedBarrier)
		{
			//Set it's active variable to false
			_cRemovedBarrier->SetActive(false);

			//Add it to the dead list and remove it from the alive list
			m_vpDeadBarriers.push_back((CBarrier*)_cRemovedBarrier);
			m_vpAliveBarriers.erase(m_vpAliveBarriers.begin() + i);

			return;
		}
	}
}

/********************************************
* AddScript(): Checks the dead list for an inactive script, sets it to active
*			    and adds it to the alive list. Then removes it from the dead list.
*
* Ins: position, _cAsset, _cAudio
*
* Outs: None
*
* Returns: CCodedScript*
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
CCodedScript* CObjectPoolSystem::AddScript(DirectX::XMFLOAT3 position, AssetManager* _cAsset, AudioSystemWwise* _cAudio)
{
	for (unsigned int i = 0; i < m_vpDeadScripts.size(); i++)
	{
		if (!m_vpDeadScripts[i]->GetActive())
		{
			m_vpDeadScripts[i]->Create(_cAsset, position);
			m_vpDeadScripts[i]->SetActive(true);
			m_vpDeadScripts[i]->SetVisible(true);

			//Add it to the alive list and remove it from dead list
			CCodedScript* tempScript = m_vpDeadScripts[i];

			m_vpAliveScripts.push_back(m_vpDeadScripts[i]);
			m_vpDeadScripts.erase(m_vpDeadScripts.begin() + i);

			return tempScript;
		}
	}

	//If something went wrong, return nullptr
	return nullptr;
}

/********************************************
* RemoveScript(): Removes the scripts passed in from the active list
*				  and adds it to the dead list.
*
* Ins: _cRemovedScript
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::RemoveScript(CObjectBase* _cRemovedScript)
{
	for (unsigned int i = 0; i < m_vpAliveScripts.size(); i++)
	{
		if (m_vpAliveScripts[i] == _cRemovedScript)
		{
			//Set it's active variable to false
			_cRemovedScript->SetActive(false);

			//Add it to the dead list and remove it from the alive list
			m_vpDeadScripts.push_back((CCodedScript*)_cRemovedScript);
			m_vpAliveScripts.erase(m_vpAliveScripts.begin() + i);

			return;
		}
	}
}

/********************************************
* AddExplosion(): Checks the dead list for an inactive explosion, sets it to active
*			    and adds it to the alive list. Then removes it from the dead list.
*
* Ins: position, _unDamage, _cAsset, _fScale, _bDamagePlayer
*
* Outs: None
*
* Returns: CExplosion*
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
CExplosion* CObjectPoolSystem::AddExplosion(DirectX::XMFLOAT3 _tPosition, unsigned int _unDamage, AssetManager* _cAsset, float _fScale, int _nType)
{
	for (unsigned int i = 0; i < m_vpDeadExplosion.size(); i++)
	{
		if (!m_vpDeadExplosion[i]->GetActive())
		{
			m_vpDeadExplosion[i]->Create(_cAsset, _tPosition, _unDamage, _fScale, (CExplosion::ExplosionType)_nType);
			m_vpDeadExplosion[i]->SetActive(true);

			//Add it to the alive list and remove it from dead list
			CExplosion* tempExplosion = m_vpDeadExplosion[i];

			m_vpAliveExplosion.push_back(m_vpDeadExplosion[i]);
			m_vpDeadExplosion.erase(m_vpDeadExplosion.begin() + i);

			return tempExplosion;
		}
	}

	//If something went wrong, return nullptr
	return nullptr;
}

/********************************************
* RemoveExplosion(): Removes the explosion passed in from the active list
*				     and adds it to the dead list.
*
* Ins: _cRemovedExplosion
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::RemoveExplosion(CObjectBase* _cRemovedExplosion)
{
	for (unsigned int i = 0; i < m_vpAliveExplosion.size(); i++)
	{
		if (m_vpAliveExplosion[i] == _cRemovedExplosion)
		{
			//Set it's active variable to false
			_cRemovedExplosion->SetActive(false);

			//Add it to the dead list and remove it from the alive list
			m_vpDeadExplosion.push_back((CExplosion*)_cRemovedExplosion);
			m_vpAliveExplosion.erase(m_vpAliveExplosion.begin() + i);

			return;
		}
	}
}

/********************************************
* AddPickup(): Checks the dead list for an inactive pickup, sets it to active
*			   and adds it to the alive list. Then removes it from the dead list.
*
* Ins: _tPosition, _cEventID, _cAsset
*
* Outs: None
*
* Returns: CPickup*
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
CPickup* CObjectPoolSystem::AddPickup(DirectX::XMFLOAT3 _tPosition, const EventID& _cEventID, AssetManager* _cAsset)
{
	for (unsigned int i = 0; i < m_vpDeadPickup.size(); i++)
	{
		if (!m_vpDeadPickup[i]->GetActive())
		{
			if (_cEventID == "DropHealth")
			{
				m_vpDeadPickup[i]->Create(_cAsset, _tPosition, CPickup::EPickupType::HEALTH_REPLENISH);
			}
			else
			{
				m_vpDeadPickup[i]->Create(_cAsset, _tPosition, CPickup::EPickupType::POTENCY_REPLENISH);
			}

			m_vpDeadPickup[i]->SetActive(true);

			//Add it to the alive list and remove it from dead list
			CPickup* tempPickup = m_vpDeadPickup[i];

			m_vpAlivePickup.push_back(m_vpDeadPickup[i]);
			m_vpDeadPickup.erase(m_vpDeadPickup.begin() + i);

			return tempPickup;
		}
	}

	//If something went wrong, return nullptr
	return nullptr;
}

/********************************************
* RemovePickup(): Removes the pickup passed in from the active list
*				  and adds it to the dead list.
*
* Ins: _cRemovePickup
*
* Outs: None
*
* Returns: None
*
* Mod. Date 08/06/2015
* Mod. Initials: MM
********************************************/
void CObjectPoolSystem::RemovePickup(CObjectBase* _cRemovePickup)
{
	for (unsigned int i = 0; i < m_vpAlivePickup.size(); i++)
	{
		if (m_vpAlivePickup[i] == _cRemovePickup)
		{
			//Set it's active variable to false
			_cRemovePickup->SetActive(false);

			//Add it to the dead list and remove it from the alive list
			m_vpDeadPickup.push_back((CPickup*)_cRemovePickup);
			m_vpAlivePickup.erase(m_vpAlivePickup.begin() + i);

			return;
		}
	}
}

CPermPowerUp* CObjectPoolSystem::AddPowerUp(DirectX::XMFLOAT3 _tPosition, AssetManager* _cAsset, AudioSystemWwise* _cAudio, unsigned int _nType)
{
	for (unsigned int i = 0; i < m_vpPermPowerUp.size(); i++)
	{
		if (!m_vpPermPowerUp[i]->GetActive())
		{
			m_vpPermPowerUp[i]->Create(_cAsset, _tPosition, _nType, _cAudio);
			m_vpPermPowerUp[i]->SetActive(true);

			//Add it to the alive list and remove it from dead list
			CPermPowerUp* tempPermPowerUp = m_vpPermPowerUp[i];

			m_vpAlivePermPowerUp.push_back(m_vpDeadPermPowerUp[i]);
			m_vpDeadPermPowerUp.erase(m_vpDeadPermPowerUp.begin() + i);

			return tempPermPowerUp;
		}
	}

	//If something went wrong, return nullptr
	return nullptr;
}

void CObjectPoolSystem::RemovePowerUp(CObjectBase* _cRemovePowerUp)
{
	for (unsigned int i = 0; i < m_vpAlivePermPowerUp.size(); i++)
	{
		if (m_vpAlivePermPowerUp[i] == _cRemovePowerUp)
		{
			//Set it's active variable to false
			_cRemovePowerUp->SetActive(false);

			//Add it to the dead list and remove it from the alive list
			m_vpDeadPermPowerUp.push_back((CPermPowerUp*)_cRemovePowerUp);
			m_vpAlivePermPowerUp.erase(m_vpAlivePermPowerUp.begin() + i);

			return;
		}
	}
}