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

HRESULT CTestObj::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	if (pArg)
		memcpy_s(&m_tTestDesc, sizeof(TESTDESC), pArg, sizeof(TESTDESC));

	m_pTransform->Set_TransformDesc(5.f, 10.f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_tTestDesc.vPosition), 1.f));
	
	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = m_tTestDesc.isTrigger;
	tColliderDesc.eRigidType = m_tTestDesc._eRigidType;
	tColliderDesc.pGameObject = this;

	CBoxCollider::DESC tBoxColliderDesc;
	tBoxColliderDesc.tColliderDesc = tColliderDesc;
	tBoxColliderDesc.vScale = { 1.f, 1.f, 1.f };
	if (FAILED(SetUp_Components(m_iSceneID, L"Proto_Component_BoxCollider", L"BoxCollider", (CComponent**)&m_pBoxCollider, &tBoxColliderDesc)))
		return E_FAIL;

	m_isMove = m_tTestDesc.isMove;

	return S_OK;
}

_int CTestObj::Tick(_double TimeDelta)
{
	move(TimeDelta);

	if(m_pBoxCollider)
		m_pBoxCollider->Tick(TimeDelta);
	if(m_pSphereCollider)
		m_pSphereCollider->Tick(TimeDelta);
	if (m_pCapsuleCollider)
		m_pCapsuleCollider->Tick(TimeDelta);

	return _int();
}

_int CTestObj::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
	return _int();
}

HRESULT CTestObj::Render()
{

	return S_OK;
}

void CTestObj::OnCollisionEnter(CCollision& _collision)
{
	if(typeid(*_collision.pGameObject) == typeid(CTestObj))
	{

	}
}

void CTestObj::OnCollisionExit(CCollision& _collision)
{
	if (typeid(*_collision.pGameObject) == typeid(CTestObj))
	{

	}
}

void CTestObj::OnTriggerEnter(CCollision& _collision)
{
	if (typeid(*_collision.pGameObject) == typeid(CTestObj))
	{

	}
}

void CTestObj::setIsMove(const _bool _isMove)
{
	m_isMove = _isMove;
}

void CTestObj::move(_double dDeltaTime)
{
	if (!m_isMove)
		return;

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

CGameObject* CTestObj::Clone(const _uint _iSceneID, void* pArg)
{
	CTestObj* pInstance = new CTestObj(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("CTestObj Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestObj::Free()
{
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pSphereCollider);
	Safe_Release(m_pCapsuleCollider);
	CGameObject::Free();
}
