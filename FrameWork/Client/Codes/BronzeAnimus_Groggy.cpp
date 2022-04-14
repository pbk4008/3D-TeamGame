#include "pch.h"
#include "BronzeAnimus_Groggy.h"
#include "Monster_BronzeAnimus.h"

CBronzeAnimus_Groggy::CBronzeAnimus_Groggy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBronzeAnimus_State(_pDevice, _pDeviceContext)
{
}

HRESULT CBronzeAnimus_Groggy::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBronzeAnimus_Groggy::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	m_fGroggyTime -= (_float)_dDeltaTime;
	cout << "±×·Î±â" << endl;

	if ((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_STUN == m_pAnimator->Get_CurrentAnimNode()
		&& m_fGroggyTime <= 0.f)
	{
		m_pAnimator->Change_LoopAnim();
	}

	return _int();
}

_int CBronzeAnimus_Groggy::LateTick(const _double& _dDeltaTime)
{				
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBronzeAnimus_Groggy::Render()
{
	return S_OK;
}

void CBronzeAnimus_Groggy::Look_Player(void)
{
	
}

void CBronzeAnimus_Groggy::Look_Monster(void)
{
	/*if (0 >= m_fGroggyTime)
	{
		m_bGroggy = true;
		m_pStateController->Change_State(L"Groggy_End");
	}*/
}

void CBronzeAnimus_Groggy::OnTriggerEnter(CCollision& collision)
{
}

HRESULT CBronzeAnimus_Groggy::EnterState()
{
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_STUN_ST);

	return S_OK;
}

HRESULT CBronzeAnimus_Groggy::ExitState()
{
	m_fGroggyTime = 5.0f;
	m_pMonster->Set_Groggy(false);
	return S_OK;
}

CBronzeAnimus_Groggy* CBronzeAnimus_Groggy::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBronzeAnimus_Groggy* pInstance = new CBronzeAnimus_Groggy(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBronzeAnimus_Groggy Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBronzeAnimus_Groggy::Free()
{
	__super::Free();
}
