#include "pch.h"
#include "Monster_Bastion_Shooter.h"
#include "Shooter_Idle.h"

CMonster_Bastion_Shooter::CMonster_Bastion_Shooter(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
{
}

CMonster_Bastion_Shooter::CMonster_Bastion_Shooter(const CMonster_Bastion_Shooter& _rhs)
	: CActor(_rhs)
{
}

HRESULT CMonster_Bastion_Shooter::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_AnimationFSM()))
		return E_FAIL;

	if (FAILED(Ready_StateFSM()))
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.f, 0.f, 5.f, 1.f));

	return S_OK;
}

_int CMonster_Bastion_Shooter::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pAnimator->Tick(_dDeltaTime);

	return _int();
}

_int CMonster_Bastion_Shooter::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) 
		return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return -1;

	return _int();
}

HRESULT CMonster_Bastion_Shooter::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) 
		return E_FAIL;
	if (FAILED(m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) 
		return E_FAIL;
	if (FAILED(m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) 
		return E_FAIL;

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
	{
		//if (FAILED(m_pModelCom->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE))) return E_FAIL;

		if (FAILED(m_pModelCom->Render(i, 0))) 
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::Ready_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components(m_iSceneID, L"Model_Monster_Bastion_Marksman", L"Model", (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModelCom;
	tDesc.pTransform = m_pTransform;

	if (FAILED(__super::SetUp_Components(m_iSceneID, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)&m_pAnimator, &tDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components(m_iSceneID, L"Proto_Component_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::Ready_AnimationFSM()
{
	CAnimation* pAnim = m_pModelCom->Get_Animation("A_Idle_CrimsonMarksman");
	//생성 하면서 연결(연결 할애, 연결 당할애, 애니메이션, 루트 애님, 트랜스폼(루트애니메이션할때 찐으로 따라감), 루프, 옵션)
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::HEAD, pAnim, true, true, true, ERootOption::XYZ)))
		return E_FAIL;

	////////////////////
	//pAnim = m_pModelCom->Get_Animation("Run_Start");
	//if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_START, (_uint)ANIM_TYPE::IDLE, pAnim, true, false, false, ERootOption::XYZ)))
	//	return E_FAIL;

	//pAnim = m_pModelCom->Get_Animation("Run_Loop");
	//if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_LOOP, (_uint)ANIM_TYPE::RUN_START, pAnim, true, false, false, ERootOption::XYZ)))
	//	return E_FAIL;

	//pAnim = m_pModelCom->Get_Animation("Run_Stop");
	//if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::RUN_END, (_uint)ANIM_TYPE::RUN_LOOP, pAnim, true, false, false, ERootOption::XYZ)))
	//	return E_FAIL;

	///////////////////////////////////////////////
	//pAnim = m_pModelCom->Get_Animation("Attack_JumpStart");
	//if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK_JUMPSTART, (_uint)ANIM_TYPE::HEAD, pAnim, true, false, false, ERootOption::XYZ)))
	//	return E_FAIL;

	//pAnim = m_pModelCom->Get_Animation("Attack_JumpLoop");
	//if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK_JUMPLOOP, (_uint)ANIM_TYPE::ATTACK_JUMPSTART, pAnim, true, false, false, ERootOption::XYZ)))
	//	return E_FAIL;

	//pAnim = m_pModelCom->Get_Animation("Attack_JumpEnd");
	//if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ATTACK_JUMPEND, (_uint)ANIM_TYPE::ATTACK_JUMPLOOP, pAnim, true, false, false, ERootOption::XYZ)))
	//	return E_FAIL;

	////////////////////////////////////
	////애니메이션 연결(연결 당할 애, 연결할 애, 쌍방으로 연결할지 안할지)
	//if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::RUN_END, false)))
	//	return E_FAIL;

	//if (FAILED(m_pAnimator->Connect_Animation((_uint)ANIM_TYPE::IDLE, (_uint)ANIM_TYPE::ATTACK_JUMPEND, false)))
	//	return E_FAIL;

	////자동으로 돌릴 애들(끝나는애, 끝나고 시작할 애)
	//m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_START, (_uint)ANIM_TYPE::RUN_LOOP);
	//m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_LOOP, (_uint)ANIM_TYPE::RUN_END);
	//m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::RUN_END, (_uint)ANIM_TYPE::IDLE);

	//m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK_JUMPSTART, (_uint)ANIM_TYPE::ATTACK_JUMPLOOP);
	//m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK_JUMPLOOP, (_uint)ANIM_TYPE::ATTACK_JUMPEND);
	//m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ATTACK_JUMPEND, (_uint)ANIM_TYPE::IDLE);

	//m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::ATTACK_JUMPSTART);


	//애니메이션 체인지(바꿀 애)
	m_pAnimator->Change_Animation((_uint)ANIM_TYPE::IDLE);

	return S_OK;
}

HRESULT CMonster_Bastion_Shooter::Ready_StateFSM()
{
	if (FAILED(m_pStateController->Add_State(L"Idle", CShooter_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	for (auto& pair : m_pStateController->Get_States())
	{
		pair.second->Set_StateController(m_pStateController);
		static_cast<CMonster_FSM*>(pair.second)->Set_Monster(this);
		static_cast<CMonster_FSM*>(pair.second)->Set_Transform(m_pTransform);
		static_cast<CMonster_FSM*>(pair.second)->Set_Model(m_pModelCom);
		static_cast<CMonster_FSM*>(pair.second)->Set_Animator(m_pAnimator);
	}
	m_pStateController->Change_State(L"Idle");

	return S_OK;
}

CMonster_Bastion_Shooter* CMonster_Bastion_Shooter::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_Bastion_Shooter* pInstance = new CMonster_Bastion_Shooter(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMonster_Bastion_Shooter Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_Bastion_Shooter::Clone(const _uint _iSceneID, void* _pArg)
{
	CMonster_Bastion_Shooter* pInstance = new CMonster_Bastion_Shooter(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CMonster_Bastion_Shooter Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Bastion_Shooter::Free()
{
	Safe_Release(m_pCollider);
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pModelCom);

	__super::Free();
}
