#include "pch.h"
#include "TestObj.h"

CTestObj::CTestObj()
	: m_pBoxCollider(nullptr)
	, m_pSphereCollider(nullptr)
	, m_pCapsuleCollider(nullptr)
{
}

CTestObj::CTestObj(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pBoxCollider(nullptr)
	, m_pSphereCollider(nullptr)
	, m_pCapsuleCollider(nullptr)
{
}

CTestObj::CTestObj(const CTestObj& rhs)
	: CGameObject(rhs)
	, m_pBoxCollider(rhs.m_pBoxCollider)
	, m_pSphereCollider(rhs.m_pSphereCollider)
	, m_pCapsuleCollider(rhs.m_pCapsuleCollider)
{
	Safe_AddRef(m_pBoxCollider);
	Safe_AddRef(m_pSphereCollider);
	Safe_AddRef(m_pCapsuleCollider);
}

HRESULT CTestObj::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTestObj::NativeConstruct(void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(pArg)))
		return E_FAIL;

	m_pTransform->SetTransformDesc(5.f, 10.f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 10.f, 1.f));
	
	TESTDESC tDesc = (*(TESTDESC*)pArg);


	if (tDesc.tSphereDesc.matTransform._11 == 0.f && tDesc.tCapsuleDesc.matTransform._11 == 0.f)
	{
		tDesc.tBoxDesc.pParent = this;
		if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"BoxCollider", L"TestCol", (CComponent * *)& m_pBoxCollider, &tDesc.tBoxDesc)))
			return E_FAIL;
	}
	else
	{
		if (tDesc.tSphereDesc.matTransform._11 != 0.f)
		{
			tDesc.tSphereDesc.pParent = this;
			if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"SphereCollider", L"TestCol", (CComponent * *)& m_pSphereCollider, &tDesc.tSphereDesc)))
				return E_FAIL;
		}
		else
		{
			tDesc.tCapsuleDesc.pParent = this;
			if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"CapsuleCollider", L"TestCol", (CComponent * *)& m_pCapsuleCollider, &tDesc.tCapsuleDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

_int CTestObj::Tick(_double TimeDelta)
{
	move(TimeDelta);

	if(m_pBoxCollider)
		m_pBoxCollider->Update(m_pTransform->Get_WorldMatrix());
	if(m_pSphereCollider)
		m_pSphereCollider->Update(m_pTransform->Get_WorldMatrix());
	if (m_pCapsuleCollider)
		m_pCapsuleCollider->Update(m_pTransform->Get_WorldMatrix());

	return _int();
}

_int CTestObj::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
	return _int();
}

HRESULT CTestObj::Render()
{
#ifdef _DEBUG
	if(m_pBoxCollider)
		m_pBoxCollider->Render(L"MainCamera");
	if (m_pSphereCollider)
		m_pSphereCollider->Render(L"MainCamera");
	if (m_pCapsuleCollider)
		m_pCapsuleCollider->Render(L"MainCamera");
#endif
	return S_OK;
}

void CTestObj::OnCollisionEnter(CGameObject* pObj)
{
	if(typeid(*pObj) == typeid(CTestObj))
	{
		if (m_pBoxCollider)
			m_pBoxCollider->Collider();
		else if (m_pSphereCollider)
			m_pSphereCollider->Collider();
		else if (m_pCapsuleCollider)
			m_pCapsuleCollider->Collider();
	}
}

void CTestObj::OnCollisionExit(CGameObject* pObj)
{
	if (typeid(*pObj) == typeid(CTestObj))
	{
		if (m_pBoxCollider)
			m_pBoxCollider->Collider();
		else if (m_pSphereCollider)
			m_pSphereCollider->Collider();
		else if (m_pCapsuleCollider)
			m_pCapsuleCollider->Collider();
	}
}

void CTestObj::OnTriggerEnter(CGameObject* pObj)
{
	if (typeid(*pObj) == typeid(CTestObj))
	{
		if (m_pBoxCollider)
			m_pBoxCollider->Collider();
		else if (m_pSphereCollider)
			m_pSphereCollider->Collider();
		else if (m_pCapsuleCollider)
			m_pCapsuleCollider->Collider();
	}
}

void CTestObj::move(_double dDeltaTime)
{
	if (g_pGameInstance->getkeyPress(DIK_RIGHT))
		m_pTransform->Go_Right(dDeltaTime);
	if (g_pGameInstance->getkeyPress(DIK_UP))
		m_pTransform->Go_Straight(dDeltaTime);
	if (g_pGameInstance->getkeyPress(DIK_LEFT))
		m_pTransform->Go_Left(dDeltaTime);
	if (g_pGameInstance->getkeyPress(DIK_DOWN))
		m_pTransform->Go_BackWard(dDeltaTime);
}

CTestObj* CTestObj::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTestObj* pInstance = new CTestObj(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTestObj Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTestObj::Clone(void* pArg)
{
	CTestObj* pInstance = new CTestObj(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CTestObj Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestObj::Free()
{
	CGameObject::Free();
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pSphereCollider);
	Safe_Release(m_pCapsuleCollider);
}
