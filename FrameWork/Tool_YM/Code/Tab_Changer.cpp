#include "pch.h"
#include "Tab_Changer.h"
#include "Changer.h"
#include "GameInstance.h"
#include "Map_Tool.h"
#include "Cam_Tool.h"
#include "MapTool_Level.h"
#include "CamTool_Level.h"
#include "Static.h"

CTab_Changer::CTab_Changer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CLevel(_pDevice, _pDeviceContext)
{
}

HRESULT CTab_Changer::Ready_Level(TAB_ID _eNextTab)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	m_eNextTAB = _eNextTab;

	m_pChanger = CChanger::Create(m_pDevice, m_pDeviceContext, m_eNextTAB);
	if (nullptr == m_pChanger)
		return E_FAIL;

	return S_OK;
}

_int CTab_Changer::Update_Level(_double _dTimeDelta)
{
	if (nullptr == m_pChanger)
		return -1;

	if (0 > __super::Tick(_dTimeDelta))
		return -1;

	if (TRUE == m_pChanger->Get_Finished())
	{
		if (FAILED(Set_ShowTab()))
			return -1;
	}
	return _int();
}

HRESULT CTab_Changer::Render_Level(void)
{
	if (nullptr == m_pChanger)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTab_Changer::Set_ShowTab(void)
{
	CLevel* pLevel = nullptr;

	switch (m_eNextTAB)
	{
	case TAB_STATIC:
		pLevel = CStatic_Level::Create(m_pDevice, m_pDeviceContext);
		break;
	case TAB_MAP:
		pLevel = CMapTool_Level::Create(m_pDevice, m_pDeviceContext);
		break;
	case TAB_CAM:
		pLevel = CCamTool_Level::Create(m_pDevice, m_pDeviceContext);
		break;
	}

	if (FAILED(g_pGameInstance->Open_Level(m_eNextTAB, pLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTab_Changer::Ready_Layer_BackGround(const _tchar* _pLayerTag)
{
	return S_OK;
}

HRESULT CTab_Changer::Ready_Layer_UI(const _tchar* _pLayerTag)
{
	return S_OK;
}

CTab_Changer* CTab_Changer::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, TAB_ID _eNextTab)
{
	CTab_Changer* pInstance = new CTab_Changer(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->Ready_Level(_eNextTab)))
	{
		MSGBOX("Faild to Create CLevel Loading");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTab_Changer::Free(void)
{
	__super::Free();
	Safe_Release(m_pChanger);
}

