#pragma once
#ifndef __TRIGGER_SYSTEM_H__
#define __TRIGGER_SYSTEM_H__

#include "Base.h"
#include "Client_Trigger.h"
#include "Stage1.h"

BEGIN(Client)
class CClient_Trigger;
class CStage1;
template<typename T>
class CTriggerSystem final : public CBase 
{
public:
	explicit CTriggerSystem() = default;
	explicit CTriggerSystem(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
		: m_pDevice(pDevice)
		, m_pDeviceContext(pDeviceContext)
		, m_iIndex(0)
	{
		Safe_AddRef(m_pDevice);
		Safe_AddRef(m_pDeviceContext);
	}
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

		_uint iSize = (_uint)m_vecTrigger.size();
		for (_uint i = 0; i < iSize; i++)
		{
			if (i >= m_iIndex)
				break;
			m_vecTrigger[i]->Tick(dDeltaTime);

			if (m_vecTrigger[i]->Get_OnTrigger() && !m_vecTrigger[i]->Get_Overlap())
			{
				m_vecTrigger[i]->Set_Overlap(true);
				
				(m_pStage->*m_TriggerFunctionArr[i].first)(m_TriggerFunctionArr[i].second);
			}
		}
		return _uint();
	}
	HRESULT Render()
	{
#ifdef _DEBUG
		for (auto& pTrigger : m_vecTrigger)
			pTrigger->Render();
#endif
		return S_OK;
	}
public:
	HRESULT Add_TriggerFuntion(void(T::* pf)(const wstring&), const wstring& pFunctionArg)
	{
		pair<void(T::*)(const wstring&), wstring > pPair = make_pair(pf, pFunctionArg);
		m_TriggerFunctionArr[m_iIndex] = pPair;
		m_iIndex++;

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
		}

		_uint iSize = (_uint)m_vecTrigger.size();
		m_TriggerFunctionArr = new pair<void(T::*)(const wstring&), wstring >[iSize];


		return S_OK;
	}
private:
	virtual void Free() override
	{
		Safe_Release(m_pDevice);
		Safe_Release(m_pDeviceContext);
		Safe_Delete_Array(m_TriggerFunctionArr);
		for (auto& pTrigger : m_vecTrigger)
			Safe_Release(pTrigger);

	}
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
private:
	T* m_pStage;
	vector<CClient_Trigger*> m_vecTrigger;
	//vector<pair<void(T::*)(const wstring&), wstring >> m_vecTriggerFunction;
	pair<void(T::*)(const wstring&), wstring >* m_TriggerFunctionArr;
	_uint m_iIndex;
};
END
#endif