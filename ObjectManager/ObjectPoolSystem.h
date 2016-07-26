#pragma once
#include "../PreCompiled.h"
/************************************************
* Filename:      ObjectPoolSystem.h
* Date:          07/22/2015
* Mod. Date:     07/22/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       To create several pools of objects
*				 for an alive and dead list.
*************************************************/

//Forward Declarations
class CBullet;
class CGrenade;
class CEnemyObject;
class CSpawner;
class CBarrier;
class CTrigger;
class AssetManager;
class CAIManager;
class CPlayerObject;
class CModel;
class CCodedScript;
class CExplosion;
class CPickup;
class EventID;
class CPermPowerUp;
class cRenderer;

class CObjectPoolSystem
{
private:
	//Pool System
	std::vector<CBullet*> m_vpBulletPool;
	std::vector<CGrenade*> m_vpGrenadePool;
	std::vector<CEnemyObject*> m_vpEnemyPool;
	std::vector<CSpawner*> m_vpSpawnerPool;
	std::vector<CTrigger*> m_vpTriggerPool;
	std::vector<CBarrier*> m_vpBarrierPool;
	std::vector<CCodedScript*> m_vpCodedScriptPool;
	std::vector<CExplosion*> m_vpExplosionPool;
	std::vector<CPickup*> m_vpPickupPool;
	std::vector<CPermPowerUp*> m_vpPermPowerUp;

	//Active List
	std::vector<CBullet*> m_vpAliveBullets;
	std::vector<CGrenade*> m_vpAliveGrenades;
	std::vector<CEnemyObject*> m_vpAliveEnemies;
	std::vector<CSpawner*> m_vpAliveSpawners;
	std::vector<CTrigger*> m_vpAliveTriggers;
	std::vector<CBarrier*> m_vpAliveBarriers;
	std::vector<CCodedScript*> m_vpAliveScripts;
	std::vector<CExplosion*> m_vpAliveExplosion;
	std::vector<CPickup*> m_vpAlivePickup;
	std::vector<CPermPowerUp*> m_vpAlivePermPowerUp;

	//Dead List
	std::vector<CBullet*> m_vpDeadBullets;
	std::vector<CGrenade*> m_vpDeadGrenades;
	std::vector<CEnemyObject*> m_vpDeadEnemies;
	std::vector<CSpawner*> m_vpDeadSpawners;
	std::vector<CTrigger*> m_vpDeadTriggers;
	std::vector<CBarrier*> m_vpDeadBarriers;
	std::vector<CCodedScript*> m_vpDeadScripts;
	std::vector<CExplosion*> m_vpDeadExplosion;
	std::vector<CPickup*> m_vpDeadPickup;
	std::vector<CPermPowerUp*> m_vpDeadPermPowerUp;

	cRenderer* renderer;

public:
	CObjectPoolSystem();
	~CObjectPoolSystem();

	void Initialize(cRenderer* renderer);
	void Shutdown();
	void ClearObjectPools();

	//Bullet Pool
	CBullet* AddBullet(float _fDamage, int _nBulletType, DirectX::XMFLOAT3 _d3dPosition, DirectX::XMFLOAT3 _d3dDirection, bool _bSpecial, AssetManager* _cAsset, AudioSystemWwise* _cAudio);
	void RemoveBullet(CObjectBase* _cRemovedBullet);

	//Grenade Pool
	CGrenade* AddGrenade(float _fDamage, float _fExplosionRadius, DirectX::XMFLOAT3 _d3dPosition, DirectX::XMFLOAT3 _d3dDirection, bool _bSpecial, AssetManager* _cAsset, AudioSystemWwise* _cAudio);
	void RemoveGrenade(CObjectBase* _cRemovedGrenade);

	//Enemy Pool
	CEnemyObject* AddEnemy(int _nEnemyType, DirectX::XMFLOAT3 _d3dPosition, CSpawner * _pcSpawner, AssetManager* _cAsset, AudioSystemWwise* _cAudio, CAIManager* _pcAIManager, CPlayerObject* _pcPlayer);
	void RemoveEnemy(CObjectBase* _cRemovedEnemy);

	//Spawner Pool
	CSpawner* AddSpawner(int _nSpawnerType, DirectX::XMFLOAT3 _d3dPosition, bool _bHasScript, AssetManager* _cAsset, AudioSystemWwise* _cAudio);
	void RemoveSpawner(CObjectBase* _cRemovedSpawner);

	//Trigger Pool
	CTrigger* AddTrigger(DirectX::XMFLOAT3 position, CModel* model, AssetManager* _cAsset, AudioSystemWwise* _cAudio);
	void RemoveTrigger(CObjectBase* _cRemovedTrigger);

	//Barrier Pool
	CBarrier* AddBarrier(DirectX::XMFLOAT3 position, AssetManager* _cAsset, AudioSystemWwise* _cAudio);
	void RemoveBarrier(CObjectBase* _cRemovedBarrier);

	//CodeScript Pool
	CCodedScript* AddScript(DirectX::XMFLOAT3 position, AssetManager* _cAsset, AudioSystemWwise* _cAudio);
	void RemoveScript(CObjectBase* _cRemovedScript);

	//Explosion Pool
	CExplosion* AddExplosion(DirectX::XMFLOAT3 _tPosition, unsigned int _unDamage, AssetManager* _cAsset, float _fScale, int _nType);
	void RemoveExplosion(CObjectBase* _cRemovedExplosion);

	//Pickup Pool
	CPickup* AddPickup(DirectX::XMFLOAT3 _tPosition, const EventID&, AssetManager* _cAsset);
	void RemovePickup(CObjectBase* _cRemovePickup);

	//Perm Power up Pool
	CPermPowerUp* AddPowerUp(DirectX::XMFLOAT3 _tPosition, AssetManager* _cAsset, AudioSystemWwise* _cAudio, unsigned int _nType);
	void RemovePowerUp(CObjectBase* _cRemovePowerUp);
};