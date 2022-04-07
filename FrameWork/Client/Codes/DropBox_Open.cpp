#include "pch.h"
#include "DropBox_Open.h"
#include "DropBox.h"

CDropBox_Open::CDropBox_Open(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CDropBox_State(pDevice, pDeviceContext)
{
}

CDropBox_Open::CDropBox_Open(const CDropBox_Open& rhs)
	: CDropBox_State(rhs)
{
}

HRESULT CDropBox_Open::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CDropBox_Open::Tick(const _double& TimeDelta)
{
	_int iProgress = __super::Tick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(TimeDelta);

	return _int();
}

_int CDropBox_Open::LateTick(const _double& TimeDelta)
{
	_int iProgress = __super::LateTick(TimeDelta);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CDropBox_Open::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropBox_Open::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_Animation(CDropBox::ANIM_TYPE::A_OPEN);

	return S_OK;
}

HRESULT CDropBox_Open::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

CDropBox_Open* CDropBox_Open::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg)
{
	CDropBox_Open* pInstance = new CDropBox_Open(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CDropBox_Open Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDropBox_Open::Free()
{
	__super::Free();
}
