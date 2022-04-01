#include "pch.h"
#include "Shooter_Attack.h"
#include "Monster_Bastion_Shooter.h"
#include "Bullet.h"
#include "HierarchyNode.h"
#include "Animation.h"

CShooter_Attack::CShooter_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMonster_FSM(_pDevice, _pDeviceContext)
	, m_dAccShotTime(0.0)
	, m_bShot(false)
{
}

HRESULT CShooter_Attack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	FSMACTORDESC tDesc = (*(FSMACTORDESC*)_pArg);
	m_pAnimator = tDesc.pAnimator;
	m_pStateController = tDesc.pController;
	m_pMonster = tDesc.pActor;
	m_pTransform = m_pMonster->Get_Transform();
	m_pModel = static_cast<CModel*>(m_pMonster->Get_Component(L"Model"));

	m_wstrTag = tDesc.pName;

	//Safe_AddRef(m_pAnimator);
	//Safe_AddRef(m_pTransform);

	if (FAILED(CMonster_FSM::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShooter_Attack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);
	
	Shot(_dDeltaTime);

	if (m_pAnimator->Get_CurrentAnimNode() == (_uint)CMonster_Bastion_Shooter::ANIM_TYPE::IDLE)
	{
		m_pStateController->Change_State(L"Idle");
	}

	return _int();
}

_int CShooter_Attack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();

}

HRESULT CShooter_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShooter_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Shooter::ANIM_TYPE::ATTACK)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShooter_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_bShot = false;
	m_dAccShotTime = 0.0;

	return S_OK;
}

HRESULT CShooter_Attack::EnterState(void* pArg)
{
	return S_OK;
}

HRESULT CShooter_Attack::ExitState(void* pArg)
{
	return S_OK;
}

_uint CShooter_Attack::Shot(_double dDeltaTime)
{
	if (!m_pAnimator)
		return -1;
	m_dAccShotTime += dDeltaTime;
	cout << "Time" << m_dAccShotTime << endl;
	if (m_dAccShotTime >= 2.5 && !m_bShot)
	{
		_uint iProgress = Create_Bullet();
		m_bShot = true;
		if (iProgress == -1)
			return -1;
	}
	return _uint();
}

_uint CShooter_Attack::Create_Bullet()
{
	if (!m_pModel)
		return 0;

	_float fRadian = XMConvertToRadians(180.f);

	CHierarchyNode* pBulletNode = m_pModel->Get_Bone("bow_end");
	_matrix vBulletMatrix = pBulletNode->Get_TransformMatrix();
	_matrix matPivot =m_pModel->Get_PivotMatrix();
	_matrix matWorld=m_pTransform->Get_WorldMatrix();
	vBulletMatrix *= matPivot;
	vBulletMatrix *= matWorld;
	_uint iSceneID = g_pGameInstance->getCurrentLevel();


	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_Bullet", L"Proto_GameObject_Shooter_Bullet", &vBulletMatrix)))
		return E_FAIL;

	return _uint();
}

CShooter_Attack* CShooter_Attack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShooter_Attack* pInstance = new CShooter_Attack(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShooter_Attack Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShooter_Attack::Free()
{
	__super::Free();
}
