#include "pch.h"
#include "Pot.h"
#include "Animation.h"
#include "EffectManager.h"
#include "Effect.h"

CPot::CPot()
	: m_pStaticModel(nullptr)
	, m_pAnimModel(nullptr)
	, m_pCollider(nullptr)
	, m_iPotType(0)
	, m_bBroken(false)
{
}

CPot::CPot(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLandScape(pDevice, pDeviceContext)
	, m_pStaticModel(nullptr)
	, m_pAnimModel(nullptr)
	, m_pCollider(nullptr)
	, m_iPotType(0)
	, m_bBroken(false)
{
}

CPot::CPot(const CPot& rhs)
	: CLandScape(rhs)
	, m_pStaticModel(rhs.m_pStaticModel)
	, m_pAnimModel(rhs.m_pAnimModel)
	, m_pCollider(rhs.m_pCollider)
	, m_iPotType(rhs.m_iPotType)
	, m_bBroken(false)
{
}

HRESULT CPot::NativeConstruct_Prototype()
{
	if (FAILED(CLandScape::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::POT;

	return S_OK;
}

HRESULT CPot::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(CLandScape::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	POTDESC tDesc = (*(POTDESC*)pArg);

	m_iPotType = tDesc.iType;
	m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&tDesc.matWorld));

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pAnimModel->SetUp_AnimationIndex(0);
	return S_OK;
}

_int CPot::Tick(_double TimeDelta)
{
	//if (g_pGameInstance->getkeyDown(DIK_O))
	//{
	//	m_pTransform->Set_State(CTransform::STATE_POSITION, g_pObserver->Get_PlayerPos());
	//	m_bRemove = false;
	//	m_bBroken = false;
	//	m_pAnimModel->Get_Animation()->Reset_Animation();
	//}
	m_pCollider->Tick(TimeDelta);
	if (m_bBroken)
	{
		if(!m_pAnimModel->Get_Animation()->Is_Finished())
			m_pAnimModel->Update_CombinedTransformationMatrix(TimeDelta);
	}
	return _int();
}

_int CPot::LateTick(_double TimeDelta)
{
	if (!g_pGameInstance->isIn_WorldFrustum(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		return 0;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	_vector vPos =XMVectorZero() ;
	if (m_bBroken && m_pAnimModel->Get_IsAnimFinished())
	{
		vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		vPos += XMVectorSet(0.f, (_float)-TimeDelta * 1.5f,0.f,0.f);
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

		m_ftimer += g_fDeltaTime;
	}
	_float fY = XMVectorGetY(vPos);

	if(m_ftimer >= 5.f)
		m_bRemove = true;
	return _int();
}

HRESULT CPot::Render()
{
	wstring wstrCam = g_pGameInstance->Get_BaseCameraTag();
	_matrix matWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix matView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCam, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix matProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCam, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (!m_bBroken)
	{
		if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_ViewMatrix", &matView, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_matrix))))
			return E_FAIL;

		for (_uint i = 0; i < m_pStaticModel->Get_MaterialCount(); ++i)
			if (FAILED(m_pStaticModel->Render(i, 2))) 	return E_FAIL;
	}
	else
	{
		if (FAILED(m_pAnimModel->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pAnimModel->SetUp_ValueOnShader("g_ViewMatrix", &matView, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pAnimModel->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_matrix))))
			return E_FAIL;

		for (_uint i = 0; i < m_pAnimModel->Get_MaterialCount(); ++i)
			if (FAILED(m_pAnimModel->Render(i, 0))) 	return E_FAIL;
	}

	return S_OK;
}
void CPot::OnTriggerEnter(CCollision& collision)
{
	if (g_pObserver->IsAttack())
	{
		m_bBroken = true;
		m_pTransform->Scaling(XMVectorSet(0.5f, 0.5f,0.5f,0.f));
		CEffectManager* pInstance = GET_INSTANCE(CEffectManager);

		CEffect* pEffect = CEffectManager::GetInstance()->Get_Effect((_uint)EFFECT::BROKEN_POD);
		pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 1.f, 0.f, 0.f });
		pEffect->Set_Reset(true);
		pEffect->setActive(true);

		PLAY_SOUND(L"Pot_Broken", CHANNEL::MidBoss_Death);
		RELEASE_INSTANCE(CEffectManager);
	}
}

HRESULT CPot::Ready_Component()
{
	switch (m_iPotType)
	{
	case 0:
		if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Pot1", L"Model_Pot", (CComponent**)&m_pStaticModel)))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Pot2", L"Model_Pot", (CComponent**)&m_pStaticModel)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Pot3", L"Model_Pot", (CComponent**)&m_pStaticModel)))
			return E_FAIL;
		break;
	}

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Pot2_Anim", L"Model_Pot_Anim", (CComponent**)&m_pAnimModel)))
		return E_FAIL;
	CCapsuleCollider::DESC tColliderDesc;

	tColliderDesc.fHeight = 1.f;
	tColliderDesc.fRadius = 0.5f;
	tColliderDesc.tColliderDesc.eRigidType = ERigidType::Static;
	tColliderDesc.tColliderDesc.fMass = 1.f;
	tColliderDesc.tColliderDesc.isGravity = true;
	tColliderDesc.tColliderDesc.isTrigger = false;
	tColliderDesc.tColliderDesc.isSceneQuery = true;
	tColliderDesc.tColliderDesc.isVisualization = true;
	tColliderDesc.tColliderDesc.pGameObject = this;
	tColliderDesc.tColliderDesc.isKinematic = false;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", L"Collider", (CComponent**)&m_pCollider, &tColliderDesc)))
		return E_FAIL;

	m_pCollider->setShapeLayer((_uint)ELayer::Pot);
	_matrix matRot = XMMatrixRotationZ(XMConvertToRadians(90.f))*XMMatrixTranslation(0.f,1.f,0.f);
	m_pCollider->setPivotMatrix(matRot);

	return S_OK;
}

CPot* CPot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CPot* pInstance = new CPot(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CPot Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPot::Clone(const _uint iSceneID, void* pArg)
{
	CPot* pInstance = new CPot(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CPot Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPot::Free()
{
	CLandScape::Free();
	Safe_Release(m_pStaticModel);
	Safe_Release(m_pAnimModel);
	Safe_Release(m_pCollider);
}
