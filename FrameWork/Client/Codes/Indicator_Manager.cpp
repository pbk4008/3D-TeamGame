#include "pch.h"
#include "Indicator_Manager.h"

CIndicator_Manager::CIndicator_Manager()
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
{
}


HRESULT CIndicator_Manager::Ready_Indicator_Manager(ID3D11Device* pDevcie, ID3D11DeviceContext* pDeviceContext)
{
	m_pDevice = pDevcie;
	m_pDeviceContext = pDeviceContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

HRESULT CIndicator_Manager::Add_Indicator(_uint iSceneID, const wstring& pLayerTag , _uint iCount)
{
	for (_uint i = 0; i < iCount; ++i)
	{
		CUI_Indicator* pIndicator = nullptr;
		//Indicator
		CUI_Indicator::UIACTIVEDESC Desc;
		_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Indicator");
		Desc.UIDesc.IDTag = 13;
		Desc.UIDesc.bMinus = false;
		Desc.UIDesc.fAngle = 0.f;
		Desc.UIDesc.fPos = { 0.f, 0.f, 0.f };
		Desc.UIDesc.fSize = { 3.f, 3.f };

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_UI_Indicator", L"Proto_GameObject_UI_Indicator", &Desc, (CGameObject**)&pIndicator)))
			return E_FAIL;

		m_vecIndicator.push_back(pIndicator);
	}
	
	return S_OK;
}

void CIndicator_Manager::Active_Indicator()
{
	OVERLAPDESC tOverlapDesc;
	tOverlapDesc.geometry = PxSphereGeometry(10.f);
	XMStoreFloat3(&tOverlapDesc.vOrigin, g_pObserver->Get_PlayerPos());
	CGameObject* pHitObject = nullptr;
	tOverlapDesc.ppOutHitObject = &pHitObject;
	tOverlapDesc.filterData.flags = PxQueryFlag::eDYNAMIC;
	if (g_pGameInstance->Overlap(tOverlapDesc))
	{
		_uint iSize = (_uint)tOverlapDesc.vecHitObjects.size();
		for (_uint i = 0; i < iSize; ++i)
		{
			CActor* pActor = static_cast<CActor*>(tOverlapDesc.vecHitObjects[i]);
			_uint iTag = tOverlapDesc.vecHitObjects[i]->getTag();

			if ((_uint)GAMEOBJECT::MONSTER_CRYSTAL == iTag || (_uint)GAMEOBJECT::MONSTER_ABERRANT == iTag || (_uint)GAMEOBJECT::MONSTER_1H == iTag ||
				(_uint)GAMEOBJECT::MONSTER_2H == iTag || (_uint)GAMEOBJECT::MONSTER_HEALER == iTag || (_uint)GAMEOBJECT::MONSTER_SHOOTER == iTag ||
				(_uint)GAMEOBJECT::MONSTER_SPEAR == iTag || (_uint)GAMEOBJECT::MONSTER_ANIMUS == iTag)
			{
				//플레이어때문에 갯수가 하나더들어옴
				if (tOverlapDesc.iHitNum - 1 >= m_iHitNum)
				{
					for (_uint i = 0; i < tOverlapDesc.iHitNum; ++i)
					{
						if ((_uint)GAMEOBJECT::PLAYER != tOverlapDesc.vecHitObjects[i]->getTag())
						{
							m_vecIndicator[i]->setActive(true);
							_vector TargetPos = tOverlapDesc.vecHitObjects[i]->Get_Transform()->Get_State(CTransform::STATE_POSITION);
							m_vecIndicator[i]->Set_TargetPos(TargetPos);
						}

						m_iHitNum = tOverlapDesc.iHitNum - 1;
					}

				}
				else if (tOverlapDesc.iHitNum - 1 < m_iHitNum)
				{
					for (_uint i = tOverlapDesc.iHitNum - 1; i < m_iHitNum; ++i)
					{
						if ((_uint)GAMEOBJECT::PLAYER != tOverlapDesc.vecHitObjects[i]->getTag())
						{
							m_vecIndicator[i]->setActive(false);
							_vector TargetPos = tOverlapDesc.vecHitObjects[i]->Get_Transform()->Get_State(CTransform::STATE_POSITION);
							m_vecIndicator[i]->Set_TargetPos(TargetPos);
						}

						m_iHitNum = tOverlapDesc.iHitNum - 1;
					}
				}
			}
			else if(1 == tOverlapDesc.iHitNum)
			{
				//범위안에 충돌된게 플레이어뿐일때 
				for (_uint i = 0; i < m_vecIndicator.size(); ++i)
				{
					m_vecIndicator[i]->setActive(false);
				}
			}
		}
	}
}

void CIndicator_Manager::Setting_TargetPos()
{

}

CUI_Indicator* CIndicator_Manager::Get_Indicator(_uint iIndicatorIndex)
{
	CUI_Indicator* pIndicator = m_vecIndicator[iIndicatorIndex];

	return pIndicator;
}

void CIndicator_Manager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	for (auto& pIndicator : m_vecIndicator)
	{
		Safe_Release(pIndicator);
	}
	m_vecIndicator.clear();
}
