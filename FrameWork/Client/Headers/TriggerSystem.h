#pragma once
#ifndef __TRIGGER_SYSTEM_H__
#define __TRIGGER_SYSTEM_H__

#include "Base.h"
#include "GameObject.h"
#include "Client_Trigger.h"

BEGIN(Client)
class CClient_Trigger;
template<typename T>
class CTriggerSystem final : public CBase
{
public:
	enum class MONSTER {MON_SWORD, MON_SHOOTER, MON_HEAL, MON_2H, MON_SPEAR
		, MON_CRYSTAL, MON_EARTH,MON_BRONZE, MON_MID, MON_BOSS, MON_END};
public:
	explicit CTriggerSystem() = default;
	explicit CTriggerSystem(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
		: m_pDevice(pDevice)
		, m_pDeviceContext(pDeviceContext)
		, m_pStage(nullptr)
		, m_bAllTriggerOn(false)
		, m_bOverlap(false)
		, m_iClearIndex(-1)
	{
		Safe_AddRef(m_pDevice);
		Safe_AddRef(m_pDeviceContext);
	}
private:
	virtual ~CTriggerSystem() = default;
public:
	HRESULT NativeConstruct(const wstring& pFilePath, T* pStage)
	{
		if (FAILED(Load_Trigger(pFilePath)))
			return E_FAIL;

		m_pStage = pStage;

		return S_OK;
	}
public:
	_uint Tick(_double dDeltaTime)
	{
		if (m_vecTrigger.empty())
			return -1;
		_bool bCheck = false;
		_uint iSize = (_uint)m_vecTrigger.size();
		for (_uint i = 0; i < iSize; i++)
		{
			if(m_vecTrigger[i]->getActive())
			{
				m_vecTrigger[i]->Tick(dDeltaTime);
				if (m_vecTrigger[i]->Get_OnTrigger() && !m_vecTrigger[i]->Get_Overlap())
				{
					m_vecTrigger[i]->Set_Overlap(true);
					(m_pStage->*m_vecTriggerFunction[i])();
					m_iClearIndex = i;
				}
				if (!m_vecClear[i])
					bCheck = true;
			}
		}
		if (!bCheck)
		{
			m_bAllTriggerOn = true;
			if (m_bAllTriggerOn && !m_bOverlap)
			{
				(m_pStage->*m_vecTriggerFunction.back())();
				m_bOverlap = true;
			}
		}

		return _uint();
	}
	HRESULT Render()
	{
#ifdef _DEBUG
		for (auto& pTrigger : m_vecTrigger)
		{
			if(pTrigger->getActive())
				pTrigger->Render();
		}
#endif
		return S_OK;
	}
public:
	void Check_Clear()
	{
		_int iSize = (_int)m_vecClear.size();

		if (m_iClearIndex < iSize)
		{
			CurrentTriggerMonsterAllDelete();
			m_vecClear[m_iClearIndex] = true;
			if (m_iClearIndex != iSize - 1)
			{
				m_vecTrigger[m_iClearIndex + 1]->setActive(true);
				m_vecTrigger[m_iClearIndex + 1]->TurnOnTrigger(true);
			}
		}
	}
	HRESULT Add_CurrentTriggerMonster(CGameObject* pMonster)
	{
		m_vecCurMonster.emplace_back(pMonster);
		return S_OK;
	}
	HRESULT Check_DeleteTriggerMonster()
	{
		auto iter_begin = m_vecCurMonster.begin();
		for (; iter_begin != m_vecCurMonster.end();)
		{
			
			if ((*iter_begin)->getRemove())
			{
				if (iter_begin == m_vecCurMonster.end() - 1)
					m_vecCurMonster.pop_back();
				else
					iter_begin = m_vecCurMonster.erase(iter_begin);
			}
			else
				iter_begin++;
		}
		return S_OK;
	}
	HRESULT CurrentTriggerMonsterAllDelete()
	{
		for (auto& pMonster : m_vecCurMonster)
			pMonster->Set_Remove(true);

		m_vecCurMonster.clear();
		return S_OK;
	}
	HRESULT Add_TriggerFuntion(void(T::* pf)())
	{
		m_vecTriggerFunction.emplace_back(pf);
		return S_OK;
	}
	HRESULT Load_Trigger(const wstring& pFilePath)
	{
		vector<TRIGGER> vecTrigger;
		if (FAILED(g_pGameInstance->LoadFile<TRIGGER>(vecTrigger, pFilePath)))
			return E_FAIL;

		CClient_Trigger* pTrigger = nullptr;

		for (auto& pTriggerData : vecTrigger)
		{
			pTrigger = g_pGameInstance->Clone_GameObject<CClient_Trigger>((_uint)SCENEID::SCENE_STATIC, L"Prototype_GameObject_Trigger", &pTriggerData);
			m_vecTrigger.emplace_back(pTrigger);
			pTrigger->setActive(false);
			pTrigger->TurnOnTrigger(false);
			m_vecClear.emplace_back(false);
		}
		m_vecTrigger[0]->setActive(true);
		m_vecTrigger[0]->TurnOnTrigger(true);

		return S_OK;
	}
	HRESULT Load_MonsterSpawnPoint(_uint iSceneID, MONSTER eType ,const wstring& pFilePath)
	{
		vector<TRIGGER> vecMonPonit;
		if (FAILED(g_pGameInstance->LoadFile<TRIGGER>(vecMonPonit,pFilePath)))
			return E_FAIL;

		for (auto pMonPoint : vecMonPonit)
		{
			pMonPoint.fTrigger_Point.y += 1.f;
			m_pVecMonsterSpawnPoint[(_uint)eType].emplace_back(pMonPoint.fTrigger_Point);
			switch (eType)
			{
			case MONSTER::MON_2H:
				g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_Bastion_2HSword", L"Proto_GameObject_Monster_Bastion_2HSword",&pMonPoint.fTrigger_Point);
				break;
			case MONSTER::MON_CRYSTAL:
				g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_Crawler", L"Proto_GameObject_Monster_Crawler", &pMonPoint.fTrigger_Point);
				break;
			case MONSTER::MON_EARTH:
				g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_EarthAberrant", L"Proto_GameObject_Monster_EarthAberrant", &pMonPoint.fTrigger_Point);
				break;
			case MONSTER::MON_SHOOTER:
				g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_Bastion_Shooter", L"Proto_GameObject_Monster_Bastion_Shooter", &pMonPoint.fTrigger_Point);
				break;
			case MONSTER::MON_HEAL:
				g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_Healer", L"Proto_GameObject_Monster_Bastion_Healer", &pMonPoint.fTrigger_Point);
				break;
			case MONSTER::MON_SWORD:
				g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_Bastion_Sword", L"Proto_GameObject_Monster_Bastion_Sword", &pMonPoint.fTrigger_Point);
				break;
			case MONSTER::MON_MID:
				g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_Boss", L"Proto_GameObject_Boss_Bastion", &pMonPoint.fTrigger_Point);
				break;
			case MONSTER::MON_BRONZE:
				g_pGameInstance->Add_GameObjectToLayer(iSceneID,L"Layer_Bronze",L"Proto_GameObject_Monster_BronzeAnimus",&pMonPoint.fTrigger_Point);
				break;
			case MONSTER::MON_SPEAR:
				g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_Spear", L"Proto_GameObject_Monster_Bastion_Spear", &pMonPoint.fTrigger_Point);
				break;
			}
		}
		return S_OK;
	}
	const vector<_float3> Get_MonsterSpawnPoint(MONSTER eType) { return m_pVecMonsterSpawnPoint[(_uint)eType]; }

private:
	virtual void Free() override
	{
		Safe_Release(m_pDevice);
		Safe_Release(m_pDeviceContext);

		m_vecTriggerFunction.clear();

		for (auto& pTrigger : m_vecTrigger)
			Safe_Release(pTrigger);
		for (_uint i = 0; i < (_uint)MONSTER::MON_END; i++)
			m_pVecMonsterSpawnPoint[i].clear();
	}
public:
	static CTriggerSystem<T>* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pFilePath, T* pStage)
	{
		CTriggerSystem<T>* pInstance= new CTriggerSystem<T>(pDevice, pDeviceContext);
		if (FAILED(pInstance->NativeConstruct(pFilePath,pStage)))
		{
			MSGBOX("TriggerSystem Create Fail");
			Safe_Release(pInstance);
		}
		return pInstance;
	}
	
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
private:
	T* m_pStage;
	vector<CClient_Trigger*> m_vecTrigger;
	vector<void(T::*)()> m_vecTriggerFunction;
	vector<_bool> m_vecClear;
	vector<_float3> m_pVecMonsterSpawnPoint[(_uint)MONSTER::MON_END];
	vector<CGameObject*> m_vecCurMonster;
	_bool m_bAllTriggerOn;
	_bool m_bOverlap;
	_int m_iClearIndex;
};
END
#endif
