#include "pch.h"
#include "UI_Skill_Meter.h"

CUI_Skill_Meter::CUI_Skill_Meter(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CUI(_pDevice, _pDeviceContext)
{
}

CUI_Skill_Meter::CUI_Skill_Meter(const CUI_Skill_Meter& _rhs)
	: CUI(_rhs)
{
}

HRESULT CUI_Skill_Meter::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Skill_Meter::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	return S_OK;
}

_int CUI_Skill_Meter::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CUI_Skill_Meter::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CUI_Skill_Meter::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CUI_Skill_Meter* CUI_Skill_Meter::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CUI_Skill_Meter* pInstance = new CUI_Skill_Meter(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Skill_Meter Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Skill_Meter::Clone(const _uint _iSceneID, void* _pArg)
{
	CUI_Skill_Meter* pInstance = new CUI_Skill_Meter(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CUI_Skill_Meter Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Skill_Meter::Free()
{
	__super::Free();
}
