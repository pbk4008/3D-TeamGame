#include "pch.h"
#include "Boss_Bastion_Judicator.h"
#include "Animation.h"
#include "ShieldBreaker.h"

CBoss_Bastion_Judicator::CBoss_Bastion_Judicator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CActor(pDevice, pDeviceContext)
{
}

CBoss_Bastion_Judicator::CBoss_Bastion_Judicator(const CBoss_Bastion_Judicator& rhs)
	:CActor(rhs)
{
}

HRESULT CBoss_Bastion_Judicator::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(Set_Animation_FSM()))
		return E_FAIL;

	CHierarchyNode* pBone = m_pModelCom->Get_BoneMatrix("weapon_r_end");
	CShieldBreaker* pWeapon = CShieldBreaker::Create(m_pDevice, m_pDeviceContext);
	pWeapon->NativeConstruct(pBone);
	pWeapon->Set_Owner(this);
	pWeapon->Set_OwnerPivotMatrix(m_pModelCom->Get_PivotMatrix());
	m_pWeapon = pWeapon;

	_vector Pos = { 0.f, 0.f, 5.f, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);

	return S_OK;
}

_int CBoss_Bastion_Judicator::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}
	if (nullptr != m_pWeapon)
	{
		if (-1 == m_pWeapon->Tick(TimeDelta))
		{
			return -1;
		}
	}

	/*if (g_pGameInstance->getkeyDown(DIK_NUMPAD9))
	{
		++itest;
		m_pModelCom->SetUp_AnimationIndex(itest);
	}*/

	m_pAnimator->Tick(TimeDelta);

	//m_pAnimator->Get_CurrentAnim();
	//m_pModelCom->Update_CombinedTransformationMatrix(TimeDelta);

	return 0;
}

_int CBoss_Bastion_Judicator::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	if (nullptr != m_pWeapon)
	{
		if (-1 == m_pWeapon->LateTick(TimeDelta))
		{
			return E_FAIL;
		}
	}

	return 0;
}

HRESULT CBoss_Bastion_Judicator::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector CamPos = g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
	{
		m_pModelCom->Render(i, 0);
	}

	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::SetUp_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Model_Boss_Bastion_Judicator", L"Model", (CComponent**)&m_pModelCom)))
	{
		return E_FAIL;
	}

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModelCom;
	tDesc.pTransform = m_pTransform;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)& m_pAnimator, &tDesc)))
	{
		return E_FAIL;
	}

	m_pModelCom->Add_Material(g_pGameInstance->Get_Material(L"MI_Bastion_Top_Tier3"), 0);
	m_pModelCom->Add_Material(g_pGameInstance->Get_Material(L"MI_Bastion_Fur_Tier3"), 1);
	m_pModelCom->Add_Material(g_pGameInstance->Get_Material(L"MI_Bastion_Down_Tier3"), 2);
	m_pModelCom->Add_Material(g_pGameInstance->Get_Material(L"MI_Bastion_Cloth_Tier3"), 3);
	
	return S_OK;
}

HRESULT CBoss_Bastion_Judicator::Set_Animation_FSM()
{
	CAnimation* pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier3.ao|A_Idle_Judicator");
	if (FAILED(m_pAnimator->Insert_Animation(IDLE, HEAD, pAnim, false, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier3.ao|A_Attack_BattleCry_Start_Judicator");
	if (FAILED(m_pAnimator->Insert_Animation(BATTLE_CRY_START, IDLE, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier3.ao|A_Attack_BattleCry_Judicator");
	if (FAILED(m_pAnimator->Insert_Animation(BATTLE_CRY, BATTLE_CRY_START, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier3.ao|A_Attack_BattleCry_Stop_Judicator");
	if (FAILED(m_pAnimator->Insert_Animation(BATTLE_CRY_STOP, BATTLE_CRY, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier3.ao|A_Run_Start_Judicator");
	if (FAILED(m_pAnimator->Insert_Animation(RUN_START, BATTLE_CRY_STOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier3.ao|A_Run_Judicator");
	if (FAILED(m_pAnimator->Insert_Animation(RUN, RUN_START, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier3.ao|A_Run_Stop_Judicator");
	if (FAILED(m_pAnimator->Insert_Animation(RUN_STOP, RUN, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;


	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier3.ao|A_Run_Stop_Judicator");
	if (FAILED(m_pAnimator->Insert_Animation(RUN_STOP, RUN_STOP, pAnim, true, true, false, ERootOption::XYZ)))
		return E_FAIL;

	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier3.ao|A_Run_Stop_Judicator");
	


	//루프하나
	m_pAnimator->Set_UpAutoChangeAnimation(IDLE, BATTLE_CRY_START);
	m_pAnimator->Set_UpAutoChangeAnimation(BATTLE_CRY_START, BATTLE_CRY);
	m_pAnimator->Set_UpAutoChangeAnimation(BATTLE_CRY, BATTLE_CRY_STOP);

	m_pAnimator->Change_Animation(BATTLE_CRY_START); //시작애니메이션 정함
	m_pAnimator->Change_AnyEntryAnimation(BATTLE_CRY_START);
	return S_OK;
}

CBoss_Bastion_Judicator* CBoss_Bastion_Judicator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CBoss_Bastion_Judicator* pInstance = new CBoss_Bastion_Judicator(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CBoss_Bastion_Judicator");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBoss_Bastion_Judicator::Clone(void* pArg)
{
	CBoss_Bastion_Judicator* pInstance = new CBoss_Bastion_Judicator(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Clone CBoss_Bastion_Judicator");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Bastion_Judicator::Free()
{
	Safe_Release(m_pModelCom);
	__super::Free();
}
