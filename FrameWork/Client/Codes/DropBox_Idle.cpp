#include "pch.h"
#include "DropBox_Idle.h"
#include "DropBox.h"

CDropBox_Idle::CDropBox_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CDropBox_State(pDevice, pDeviceContext)
{
}

CDropBox_Idle::CDropBox_Idle(const CDropBox_Idle& rhs)
	: CDropBox_State(rhs)
{
}

HRESULT CDropBox_Idle::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CDropBox_Idle::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	return _int();
}

_int CDropBox_Idle::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;


	return _int();
}

HRESULT CDropBox_Idle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropBox_Idle::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropBox_Idle::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

CDropBox_Idle* CDropBox_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CDropBox_Idle* pInstance = new CDropBox_Idle(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CDropBox_Idle Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDropBox_Idle::Free()
{
	__super::Free();
}
