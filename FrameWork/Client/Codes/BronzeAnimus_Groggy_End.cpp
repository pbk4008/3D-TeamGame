#include "pch.h"
#include "BronzeAnimus_Groggy_End.h"
#include "Monster_BronzeAnimus.h"
#include "Animation.h"

CBronzeAnimus_Groggy_End::CBronzeAnimus_Groggy_End(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBronzeAnimus_State(_pDevice, _pDeviceContext)
{
}

HRESULT CBronzeAnimus_Groggy_End::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBronzeAnimus_Groggy_End::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBronzeAnimus_Groggy_End::LateTick(const _double& _dDeltaTime)
{				
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBronzeAnimus_Groggy_End::Render()
{
	return S_OK;
}

void CBronzeAnimus_Groggy_End::Look_Player(void)
{
}

void CBronzeAnimus_Groggy_End::Look_Monster(void)
{
	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
		m_pStateController->Change_State(L"A_Idle_Battle");
}

void CBronzeAnimus_Groggy_End::OnTriggerEnter(CCollision& collision)
{
}

HRESULT CBronzeAnimus_Groggy_End::EnterState()
{
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_STUN_ED);

	return S_OK;
}

HRESULT CBronzeAnimus_Groggy_End::ExitState()
{
	return S_OK;
}

CBronzeAnimus_Groggy_End* CBronzeAnimus_Groggy_End::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBronzeAnimus_Groggy_End* pInstance = new CBronzeAnimus_Groggy_End(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBronzeAnimus_Groggy_End Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBronzeAnimus_Groggy_End::Free()
{
	__super::Free();
}
