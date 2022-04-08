#include "pch.h"
#include "UI_Indexes.h"

CUI_Indexes::CUI_Indexes(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{
}

CUI_Indexes::CUI_Indexes(const CUI_Indexes& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Indexes::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Indexes::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	this->setActive(false);

	return S_OK;
}

_int CUI_Indexes::Tick(_double dDeltaTime)
{
	return _int();
}

void CUI_Indexes::Show(void)
{
	/* show Menu Button*/
}

void CUI_Indexes::Hide(void)
{
}

void CUI_Indexes::MenuAllOff(void)
{
}

void CUI_Indexes::Click_Equipment(void)
{
}

void CUI_Indexes::Click_Armory(void)
{
}

void CUI_Indexes::SetMother(CGameObject* _pObj)
{
}

void CUI_Indexes::SetButton_Equipment(CGameObject* _pObj)
{
}

void CUI_Indexes::SetButton_Armory(CGameObject* _pObj)
{
}

CUI_Indexes* CUI_Indexes::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Indexes* pInstance = new CUI_Indexes(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Indexes Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Indexes::Clone(const _uint iSceneID, void* pArg)
{
	CUI_Indexes* pInstance = new CUI_Indexes(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_Indexes Clone Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Indexes::Free()
{
	__super::Free();
}
