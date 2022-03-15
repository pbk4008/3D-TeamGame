#include "pch.h"
#include "Monster_Bastion_2HSword.h"

/* for. Idle */
#include "Bastion_2HSword_Idle.h"

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

	return S_OK;
}

_int CMonster_Bastion_2HSword::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress) return iProgress;

	m_pAnimationController->Tick(_dDeltaTime);

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

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_YM, L"Com_AnimationController", L"AnimationController", (CComponent**)&m_pAnimationController)))
		return E_FAIL;
	m_pAnimationController->Set_GameObject(this);
	m_pAnimationController->Set_Model(m_pModel);
	m_pAnimationController->Set_Transform(m_pTransform);

	// 스테이트 컨트롤러
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_YM, L"Com_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

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

HRESULT CMonster_Bastion_2HSword::Ready_States()
{
#pragma region Default
	if (FAILED(m_pStateController->Add_State(L"Idle", CBastion_2HSword_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion

	for (auto& pair : m_pStateController->Get_States())
	{
		pair.second->Set_StateController(m_pStateController);
		static_cast<CState_Monster*>(pair.second)->Set_Monster(this);
		static_cast<CState_Monster*>(pair.second)->Set_Transform(m_pTransform);
		static_cast<CState_Monster*>(pair.second)->Set_Model(m_pModel);
		static_cast<CState_Monster*>(pair.second)->Set_AnimationController(m_pAnimationController);
	}
	m_pStateController->Change_State(L"Idle");

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
	Safe_Release(m_pAnimationController);
	Safe_Release(m_pModel);

	__super::Free();
}
