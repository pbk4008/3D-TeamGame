#include "pch.h"
#include "Bastion_2HSword_Chaser.h"

/* Monster List */
#include "Monster_Bastion_2HSword.h"

CBastion_2HSword_Chaser::CBastion_2HSword_Chaser(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_2HSword_State(_pDevice, _pDeviceContext)
{
}

CBastion_2HSword_Chaser::CBastion_2HSword_Chaser(const CBastion_2HSword_Chaser& _rhs)
	: CBastion_2HSword_State(_rhs)
{
}

HRESULT CBastion_2HSword_Chaser::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_2HSword_Chaser::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	cout << "Chaser" << endl;
	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	m_pAnimator->Tick(_dDeltaTime);

	if(m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_WALK_FWD)
		Chase(_dDeltaTime);

	return _int();
}

_int CBastion_2HSword_Chaser::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_2HSword_Chaser::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_2HSword_Chaser::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_2HSword::ANIM_TYPE::A_WALK_FWD_ST);

	return S_OK;
}

HRESULT CBastion_2HSword_Chaser::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBastion_2HSword_Chaser::Look_Player(void)
{
	//if (!m_bTargetOn)
	//if(m_pOwner->get_Target())
	//	m_pStateController->Change_State(L"Idle");
}

void CBastion_2HSword_Chaser::Look_Monster(void)
{
}

void CBastion_2HSword_Chaser::Chase(_double dDeltaTime)
{
	_vector vPlayerPos = g_pObserver->Get_PlayerPos();
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDir = vPlayerPos - vPos;
	vDir = XMVector3Normalize(vDir);

	vDir *= (_float)dDeltaTime * 3.f;

	m_pTransform->Add_Velocity(vDir);

}

CBastion_2HSword_Chaser* CBastion_2HSword_Chaser::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_2HSword_Chaser* pInstance = new CBastion_2HSword_Chaser(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_2HSword_Chaser Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBastion_2HSword_Chaser::Free()
{
	__super::Free();
}
