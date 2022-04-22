#include "pch.h"
#include "BronzeAnimus_Excution.h"
#include "Animation.h"

/* Monster List */
#include "Monster_BronzeAnimus.h"

CBronzeAnimus_Excution::CBronzeAnimus_Excution(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBronzeAnimus_State(_pDevice, _pDeviceContext)
{
}

CBronzeAnimus_Excution::CBronzeAnimus_Excution(const CBronzeAnimus_Excution& _rhs)
	: CBronzeAnimus_State(_rhs)
{
}

HRESULT CBronzeAnimus_Excution::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBronzeAnimus_Excution::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);

	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBronzeAnimus_Excution::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBronzeAnimus_Excution::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBronzeAnimus_Excution::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_EXCUTION);

	return S_OK;
}

HRESULT CBronzeAnimus_Excution::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBronzeAnimus_Excution::EnterState(void* pArg)
{

	return S_OK;
}

HRESULT CBronzeAnimus_Excution::ExitState(void* pArg)
{
	return S_OK;
}

void CBronzeAnimus_Excution::Look_Player(void)
{

}

void CBronzeAnimus_Excution::Look_Monster(void)
{

}

CBronzeAnimus_Excution* CBronzeAnimus_Excution::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBronzeAnimus_Excution* pInstance = new CBronzeAnimus_Excution(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBronzeAnimus_Excution Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBronzeAnimus_Excution::Free()
{
	__super::Free();
}
