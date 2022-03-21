#include "pch.h"
#include "Bastion_Healer_State.h"
#include "Monster_Bastion_Healer.h"
#include "Animation.h"

CBastion_Healer_State::CBastion_Healer_State(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
{
}

CBastion_Healer_State::CBastion_Healer_State(const CBastion_Healer_State& _rhs)
	: CMonster_FSM(_rhs)
{
}

HRESULT CBastion_Healer_State::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Healer_State::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	//_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	//_fvector vDist = vMonsterPos - g_pObserver->Get_PlayerPos();
	//_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	//m_bTargetOn = false;
	//m_bAttackOn = false;
	//m_bPlayerAttack = false;
	//m_bRageOn = false;

	//if (g_pObserver->Get_IsAttack())
	//	m_bPlayerAttack = true;

	//if (1.0f < fDistToPlayer && 10.0f > fDistToPlayer)
	//{
	//	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	//	m_bTargetOn = true;
	//}

	//else if (10.0f < fDistToPlayer && 15.0f > fDistToPlayer)
	//	m_bRageOn = true;

	//if (2.0f >= fDistToPlayer)
	//	m_bAttackOn = true;

	//if (m_bPlayerAttack)
	//{
	//	static_cast<CMonster_Bastion_2HSword*>(m_pMonster)->m_iHp += -1;
	//	m_pStateController->Change_State(L"Hit");
	//	//공격여부로만 판별하다보니 아직은 Hp가 1씩 깍이진 않음
	//}

	return _int();
}

_int CBastion_Healer_State::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	return _int();
}

HRESULT CBastion_Healer_State::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	Render_Debug();
#endif

	return S_OK;
}

HRESULT CBastion_Healer_State::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_Healer_State::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

/* 플레이어 상태 추적 */
void CBastion_Healer_State::Look_Player(void)
{
	
}

void CBastion_Healer_State::Look_Monster(void)
{

}

HRESULT CBastion_Healer_State::Render_Debug()
{
	wstring wstrChaserOn = L"Target On : ";
	wstring wstrIsChaser;

	if (m_bTargetOn)
		wstrIsChaser = L"TRUE";
	else
		wstrIsChaser = L"FALSE";

	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrChaserOn + wstrIsChaser).c_str(), _float2(950.f, 120.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	//////////////////

	wstring wstrAttackOn = L"Attack On : ";
	wstring wstrIsAttack;

	if (m_bAttackOn)
		wstrIsAttack = L"TRUE";
	else
		wstrIsAttack = L"FALSE";

	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(0.f, 1.0f, 0.f, 1.f), (wstrAttackOn + wstrIsAttack).c_str(), _float2(950.f, 140.f), _float2(0.6f, 0.6f))))
		return E_FAIL;

	return S_OK;
}

CBastion_Healer_State* CBastion_Healer_State::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Healer_State* pInstance = new CBastion_Healer_State(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Healer_State Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_Healer_State::Free()
{
	__super::Free();
	CAnimator* m_pAnimator = nullptr;
	CActor* m_pMonster = nullptr;
	CTransform* m_pTransform = nullptr;
	CModel* m_pModel = nullptr;
}
