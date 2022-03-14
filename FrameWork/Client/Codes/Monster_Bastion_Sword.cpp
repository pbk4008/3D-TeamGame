#include "pch.h"
#include "Monster_Bastion_Sword.h"
#include "Animation.h"

CMonster_Bastion_Sword::CMonster_Bastion_Sword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CActor(_pDevice, _pDeviceContext)
{
}

CMonster_Bastion_Sword::CMonster_Bastion_Sword(const CMonster_Bastion_Sword& _rhs)
	:CActor(_rhs)
{
}

HRESULT CMonster_Bastion_Sword::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMonster_Bastion_Sword::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(Set_Animation_FSM()))
		return E_FAIL;

	_vector Pos = { 0.f, 0.f, 10.f, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);
	return S_OK;
}

_int CMonster_Bastion_Sword::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
	{
		return -1;
	}

	m_pAnimator->Tick(_dDeltaTime);
	return 0;
}

_int CMonster_Bastion_Sword::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
	{
		return -1;
	}

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return 0;
}

HRESULT CMonster_Bastion_Sword::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector CamPos = g_pGameInstance->Get_CamPosition(L"MainCamera");

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
	{
		m_pModelCom->Render(i, 0);
	}

	return S_OK;
}

HRESULT CMonster_Bastion_Sword::SetUp_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Model_Monster_Bastion_Sword", L"Model", (CComponent**)&m_pModelCom)))
	{
		return E_FAIL;
	}

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModelCom;
	tDesc.pTransform = m_pTransform;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Com_Animator", L"Com_AnimationController", (CComponent**)& m_pAnimator, &tDesc)))
	{
		return E_FAIL;
	}

	m_pModelCom->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Bastion_Sword_Top"), 0);
	m_pModelCom->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Bastion_Sword_Down"), 1);
	m_pModelCom->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Bastion_Sword_Cloth"), 2);
	
	
	
	//vector<CAnimation*> vecAnimation = m_pModelCom->Get_Animations();

	
	/*_uint iIndex = 1;
	for (auto& pAnim : vecAnimation)
	{
		cout << iIndex << ". ";
		cout << pAnim->Get_Name() << endl;
		iIndex++;
	}*/


	return S_OK;
}

HRESULT CMonster_Bastion_Sword::Set_Animation_FSM()
{
	CAnimation* pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier1_Full.ao|A_Run_Start_Swordsworn");
	if (FAILED(m_pAnimator->Insert_Animation(L"RunStart", L"Head", pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier1_Full.ao|A_Run_Fwd_Swordsworn");
	if (FAILED(m_pAnimator->Insert_Animation(L"RunLoop", L"RunStart", pAnim, true, false, true, ERootOption::XYZ)))
		return E_FAIL;
	pAnim = m_pModelCom->Get_Animation("SK_Bastion_Tier1_Full.ao|A_Run_Stop_Swordsworn");
	if (FAILED(m_pAnimator->Insert_Animation(L"RunStop", L"RunLoop", pAnim, true, false, false, ERootOption::XYZ)))
		return E_FAIL;

	m_pAnimator->Set_UpAutoChangeAnimation(L"RunStart", L"RunLoop");
	m_pAnimator->Set_UpAutoChangeAnimation(L"RunLoop", L"RunStop");

	m_pAnimator->Change_Animation(L"RunStart");



	return S_OK;
}

CMonster_Bastion_Sword* CMonster_Bastion_Sword::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_Bastion_Sword* pInstance = new CMonster_Bastion_Sword(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMonster_Bastion_Sword");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_Bastion_Sword::Clone(void* _pArg)
{
	CMonster_Bastion_Sword* pInstance = new CMonster_Bastion_Sword(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("Failed to Creating Clone CMonster_Bastion_Sword");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Bastion_Sword::Free()
{
	Safe_Release(m_pModelCom);
	__super::Free();
}
