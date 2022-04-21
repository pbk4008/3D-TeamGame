#include "pch.h"
#include "BronzeAnimus_Excution.h"
#include "Animation.h"

/* Monster List */
#include "Monster_BronzeAnimus.h"

CBraonzeAnimus_Excution::CBraonzeAnimus_Excution(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBronzeAnimus_State(_pDevice, _pDeviceContext)
{
}

CBraonzeAnimus_Excution::CBraonzeAnimus_Excution(const CBraonzeAnimus_Excution& _rhs)
	: CBronzeAnimus_State(_rhs)
{
}

HRESULT CBraonzeAnimus_Excution::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBraonzeAnimus_Excution::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);

	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CBraonzeAnimus_Excution::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBraonzeAnimus_Excution::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBraonzeAnimus_Excution::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_BronzeAnimus::ANIM_TYPE::A_EXCUTION);

	return S_OK;
}

HRESULT CBraonzeAnimus_Excution::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBraonzeAnimus_Excution::EnterState(void* pArg)
{

	return S_OK;
}

HRESULT CBraonzeAnimus_Excution::ExitState(void* pArg)
{
	return S_OK;
}

void CBraonzeAnimus_Excution::Look_Player(void)
{

}

void CBraonzeAnimus_Excution::Look_Monster(void)
{

}

CBraonzeAnimus_Excution* CBraonzeAnimus_Excution::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBraonzeAnimus_Excution* pInstance = new CBraonzeAnimus_Excution(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBraonzeAnimus_Excution Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBraonzeAnimus_Excution::Free()
{
	__super::Free();
}
