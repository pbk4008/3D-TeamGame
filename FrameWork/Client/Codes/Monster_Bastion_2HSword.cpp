#include "pch.h"
#include "Monster_Bastion_2HSword.h"

/* for. Weapon */
#include "RetributionBlade.h"

CMonster_Bastion_2HSword::CMonster_Bastion_2HSword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
{
}

CMonster_Bastion_2HSword::CMonster_Bastion_2HSword(const CMonster_Bastion_2HSword& _rhs)
	: CActor(_rhs)
{
}

HRESULT CMonster_Bastion_2HSword::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Bastion_2HSword::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg))) return E_FAIL;

	if (FAILED(Ready_Components())) return E_FAIL;
	if (FAILED(Ready_Weapon())) return E_FAIL;
	if (FAILED(Ready_AnimFSM())) return E_FAIL;

	//m_pAnimator->Change_Animation((_uint)ANIM_TYPE::ANIM_IDLE);

	return S_OK;
}

_int CMonster_Bastion_2HSword::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress) return iProgress;

	m_pAnimator->Tick(_dDeltaTime);
	
	// 무기 업뎃
	if (m_pCurWeapon)
	{
		iProgress = m_pCurWeapon->Tick(_dDeltaTime);
		if (NO_EVENT != iProgress) return iProgress;
	}
	return _int();
}

_int CMonster_Bastion_2HSword::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this))) return -1;

	// 무기 레잇업뎃
	if (m_pCurWeapon)
	{
		iProgress = m_pCurWeapon->LateTick(_dDeltaTime);
		if (NO_EVENT != iProgress) return iProgress;
	}

	Check_DistanceForPlayer();

	return _int();
}

HRESULT CMonster_Bastion_2HSword::Render()
{
	if (FAILED(__super::Render())) return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		//if (FAILED(m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE))) return E_FAIL;

		if (FAILED(m_pModel->Render(i, 0))) return E_FAIL;
	}
	return S_OK;
}

void CMonster_Bastion_2HSword::Check_DistanceForPlayer(void)
{
	CTransform* pPlayerTransCom = (CTransform*)g_pGameInstance->Get_Component((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Silvermane", L"Com_Transform");
	_float		fDistToPlayer = 0.f;

	_fvector vPlayerPos = pPlayerTransCom->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vMonsterPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);

	_fvector vDist = vMonsterPos - vPlayerPos;
	fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (2.0f > fDistToPlayer)
	{
		m_pTransform->Face_Target(vPlayerPos);
		m_pAnimator->Change_Animation((_uint)ANIM_TYPE::ANIM_ATTK);
	}
}

HRESULT CMonster_Bastion_2HSword::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 10.f;
	transformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	m_pTransform->Set_TransformDesc(transformDesc);
	_float4 vPosition = { -3.f, 0.f, 3.f, 1.f };


	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_YM, L"Model_Bastion_2HSword", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_BastionTierII_Top"), 0);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_BastionTierII_Down"), 1);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_BastionTierII_Fur"), 2);

	m_AanimDesc.pModel = m_pModel;
	m_AanimDesc.pTransform = m_pTransform;

	//m_pAnimator
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_YM, L"Proto_Component_Animator", L"Animator", (CComponent**)&m_pAnimator, &m_AanimDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Bastion_2HSword::Ready_Weapon()
{
	CHierarchyNode* pWeaponBone = m_pModel->Get_BoneMatrix("weapon_r_end_end");
	CWeapon* pWeapon = nullptr;

	//RetributionBlade
	pWeapon = CRetributionBlade::Create(m_pDevice, m_pDeviceContext);
	pWeapon->NativeConstruct(pWeaponBone);
	pWeapon->Set_Owner(this);
	pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	m_umapWeapons.emplace(L"RetributionBlade", pWeapon);
	m_pCurWeapon = pWeapon;

	return S_OK;
}

HRESULT CMonster_Bastion_2HSword::Ready_AnimFSM(void)
{
	/* 	##1.추가하는 애니메이션 태그
		##2.연결하고자 하는 애니메이션 태그
		##3.애니메이션
		##4.루트 애님
		##5.루트 애님시 트랜스폼이 따라가게 할 것인지(제자리)
		##6.루프설정
		##7.루트애님 옵션, 막고자 하는 축설정
		##8.쌍방연결
	*/
	CAnimation* pAnimation = m_pModel->Get_Animation("A_Idle");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ANIM_IDLE, (_uint)ANIM_TYPE::ANIM_HEAD, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ, FALSE)))
		return E_FAIL;

	pAnimation = m_pModel->Get_Animation("A_Attack_S3");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::ANIM_ATTK, (_uint)ANIM_TYPE::ANIM_IDLE, pAnimation, TRUE, FALSE, FALSE, ERootOption::XYZ, TRUE)))
		return E_FAIL;

	/* ##1.끝나는 애님 ##2.루트할 애님  */
	m_pAnimator->Set_UpAutoChangeAnimation((_uint)ANIM_TYPE::ANIM_ATTK, (_uint)ANIM_TYPE::ANIM_IDLE);

	return S_OK;
}

CMonster_Bastion_2HSword* CMonster_Bastion_2HSword::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_Bastion_2HSword* pInstance = new CMonster_Bastion_2HSword(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMonster_Bastion_2HSword Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_Bastion_2HSword::Clone(void* _pArg)
{
	CMonster_Bastion_2HSword* pInstance = new CMonster_Bastion_2HSword(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CMonster_Bastion_2HSword Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Bastion_2HSword::Free()
{
	Safe_Release(m_pModel);
	Safe_Release(m_pAnimator);

	__super::Free();
}
