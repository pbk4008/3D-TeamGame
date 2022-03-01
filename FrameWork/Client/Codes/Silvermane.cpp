#include "pch.h"
#include "..\Headers\Silvermane.h"

#pragma region 스테이트들

#include "Silvermane_Idle.h"

#include "Silvermane_JogBwd.h"
#include "Silvermane_JogBwdStart.h"
#include "Silvermane_JogBwdPivot180.h"

#include "Silvermane_JogFwd.h"
#include "Silvermane_JogFwdPivot180.h"
#include "Silvermane_JogFwdStart.h"
#include "Silvermane_JogFwdStop.h"

#include "Silvermane_JogLeft.h"
#include "Silvermane_JogLeftPivot180.h"
#include "Silvermane_JogLeftStart.h"

#include "Silvermane_JogRight.h"
#include "Silvermane_JogRightPivot180.h"
#include "Silvermane_JogRightStart.h"

#pragma endregion


CSilvermane::CSilvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
{
}

CSilvermane::CSilvermane(const CSilvermane& _rhs)
	: CActor(_rhs)
{
}

HRESULT CSilvermane::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSilvermane::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_States()))
	{
		return E_FAIL;
	}

	

	return S_OK;
}

_int CSilvermane::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
	{
		return -1;
	}

	return _int();
}

_int CSilvermane::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
	{
		return -1;
	}

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return _int();
}

HRESULT CSilvermane::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSilvermane::Ready_Components()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_JS, L"Model_Silvermane", L"Model", (CComponent**)&m_pModel)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_JS, L"StateController", L"StateController", (CComponent**)&m_pStateController)))
	{
		return E_FAIL;
	}
	m_pStateController->Set_GameObject(this);

	return S_OK;
}

HRESULT CSilvermane::Ready_States()
{
	if (FAILED(m_pStateController->Add_State(L"Idle", CSilvermane_Idle::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogBwd", CSilvermane_JogBwd::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogBwdStart", CSilvermane_JogBwdStart::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogBwdPivot180", CSilvermane_JogBwdPivot180::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogFwd", CSilvermane_JogFwd::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogFwdPivot180", CSilvermane_JogFwdPivot180::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogFwdStart", CSilvermane_JogFwdStart::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogFwdStop", CSilvermane_JogFwdStop::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogLeft", CSilvermane_JogLeft::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogLeftPivot180", CSilvermane_JogLeftPivot180::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogLeftStart", CSilvermane_JogLeftStart::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogRight", CSilvermane_JogRight::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogRightPivot180", CSilvermane_JogRightPivot180::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pStateController->Add_State(L"JogRightStart", CSilvermane_JogRightStart::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	for (auto& pair : m_pStateController->Get_States())
	{
		pair.second->Set_StateController(m_pStateController);
		static_cast<CState_Silvermane*>(pair.second)->Set_Silvermane(this);
		static_cast<CState_Silvermane*>(pair.second)->Set_Transform(m_pTransform);
		static_cast<CState_Silvermane*>(pair.second)->Set_Model(m_pModel);
	}
	return S_OK;
}

CTransform* CSilvermane::Get_Transform() const
{
	return m_pTransform;
}

CSilvermane* CSilvermane::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CSilvermane* pInstance = new CSilvermane(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CSilvermane Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSilvermane::Clone(void* _pArg)
{
	CSilvermane* pInstance = new CSilvermane(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CTestObj Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSilvermane::Free()
{
	Safe_Release(m_pStateController);
	Safe_Release(m_pModel);

	__super::Free();
}
