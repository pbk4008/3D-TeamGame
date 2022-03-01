#include "pch.h"
#include "TestObj.h"

CTestObj::CTestObj()
	: m_pCollider(nullptr)
{
}

CTestObj::CTestObj(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pCollider(nullptr)
{
}

CTestObj::CTestObj(const CTestObj& rhs)
	: CGameObject(rhs)
	, m_pCollider(rhs.m_pCollider)
{
	Safe_AddRef(m_pCollider);
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

	/*CBoxCollider::BOXDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.eRot = CBoxCollider::ROTATE::ROT_AABB;
	tDesc.fExtends = _float3(0.5f, 0.5f, 0.5f);
	tDesc.tColDesc.bGravity = false;
	tDesc.tColDesc.bKinematic = false;
	XMStoreFloat3(&tDesc.tColDesc.fPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
	tDesc.tColDesc.eType = CPhysicsXSystem::ACTORTYPE::ACTOR_DYNAMIC;*/
	
	TESTDESC tDesc = (*(TESTDESC*)pArg);

	tDesc.tBoxDesc.pParent = this;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"BoxCollider", L"TestCol", (CComponent**)&m_pCollider, &tDesc.tBoxDesc)))
		return E_FAIL;


	return S_OK;
}

_int CTestObj::Tick(_double TimeDelta)
{
	move(TimeDelta);

	m_pCollider->Update(m_pTransform->Get_WorldMatrix());

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
	m_pCollider->Render(L"MainCamera");
#endif
	return S_OK;
}

void CTestObj::OnCollisionEnter(CGameObject* pObj)
{
	if (typeid(*pObj) == typeid(CTestObj))
		m_pCollider->Collider();
}

void CTestObj::OnCollisionExit(CGameObject* pObj)
{
	if (typeid(*pObj) == typeid(CTestObj))
		m_pCollider->Collider();
}

void CTestObj::OnTriggerEnter(CGameObject* pObj)
{
	if (typeid(*pObj) == typeid(CTestObj))
		m_pCollider->Collider();
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
	Safe_Release(m_pCollider);
}
