#include "pch.h"
#include "Client_Trigger.h"
#include "GameInstance.h"
#include "MeshContainer.h"
#include "Gizmo.h"

CClient_Trigger::CClient_Trigger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
	, m_pGizmo(nullptr)
	, m_bOnTrigger(false)
	, m_pBoxCollider(nullptr)
	, m_bPick(false)
	, m_bOverlap(false)
{
	ZeroMemory(&m_matWorld, sizeof(_float4x4));
	ZeroMemory(&m_fLocalMouse, sizeof(_float3));
	ZeroMemory(&m_vRayPos, sizeof(_float3));
}

CClient_Trigger::CClient_Trigger(const CClient_Trigger& _rhs)
	: CGameObject(_rhs)
	, m_pGizmo(_rhs.m_pGizmo)
	, m_bOnTrigger(_rhs.m_bOnTrigger)
	, m_pBoxCollider(_rhs.m_pBoxCollider)
	, m_bPick(_rhs.m_bPick)
	, m_bOverlap(_rhs.m_bOverlap)
{
	Safe_AddRef(m_pGizmo);
	Safe_AddRef(m_pBoxCollider);
}

HRESULT CClient_Trigger::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::TRIGGER;

	return S_OK;
}

HRESULT CClient_Trigger::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	m_TriggerDesc = *(TRIGGER*)pArg;
	
	XMStoreFloat4x4(&m_matWorld,m_pTransform->Get_WorldMatrix());
	m_matWorld._41 = m_TriggerDesc.fTrigger_Point.x;
	m_matWorld._42 = m_TriggerDesc.fTrigger_Point.y;
	m_matWorld._43 = m_TriggerDesc.fTrigger_Point.z;

	m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&m_matWorld));
	m_pTransform->Set_TransformDesc(10.0f, XMConvertToRadians(45.f));

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CClient_Trigger::Tick(_double TimeDelta)
{
	m_pBoxCollider->Tick(TimeDelta);

	return _int();
}

_int CClient_Trigger::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
	return _int();
}

HRESULT CClient_Trigger::Render()
{
	Set_WVPMatrix();

	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	switch ((TRIGGERTYPE)m_TriggerDesc.eTrigger_Type)
	{
	case TRIGGERTYPE::TRIGGER_LOD:
		m_pGizmo->DrawCapsule(m_pTransform->Get_WorldMatrix(), wstrCamTag, _fvector{0.129f, 0.65f, 0.929f, 1.0f});
		break;
	case TRIGGERTYPE::TRIGGER_LIGHT:
		m_pGizmo->DrawSphere(m_pTransform->Get_WorldMatrix(), wstrCamTag, _fvector{ 0.96f, 0.98f, 0.074f, 1.0f });
		break;
	case TRIGGERTYPE::TRIGGER_SCENE:
		m_pGizmo->DrawCube(m_pTransform->Get_WorldMatrix(), wstrCamTag, _fvector{ 1.0, 0.0f, 0.0f, 1.0f });
		break;
	case TRIGGERTYPE::TRIGGER_MONSTER:
		m_pGizmo->DrawCube(m_pTransform->Get_WorldMatrix(), wstrCamTag, _fvector{ 0.309f, 0.933f, 0.125f, 1.0f });
		break;
	case TRIGGERTYPE::TRIGGER_QUEST:
		m_pGizmo->DrawCube(m_pTransform->Get_WorldMatrix(), wstrCamTag, _fvector{ 0.501f, 0.082f, 0.0f, 1.0f });
		break;
	}
	return S_OK;
}

void CClient_Trigger::OnTriggerEnter(CCollision& collision)
{
	if (collision.pGameObject->getTag() == (_uint)GAMEOBJECT::PLAYER)
	{
		if(!m_bOnTrigger)
			m_bOnTrigger = true;
	}
}

HRESULT CClient_Trigger::SetUp_Components()
{
	/* Com_Gizmo*/
	m_pGizmo = (CGizmo*)g_pGameInstance->Clone_Component((_uint)LEVEL_ID::LEVEL_STATIC, L"Proto_Component_Gizmo");
	if (nullptr == m_pGizmo)
		return E_FAIL;

	CBoxCollider::DESC tDesc;

	tDesc.tColliderDesc.eRigidType = ERigidType::Static;
	tDesc.tColliderDesc.isGravity = false;
	tDesc.tColliderDesc.isKinematic = false;
	tDesc.tColliderDesc.isSceneQuery = false;
	tDesc.tColliderDesc.isTrigger = true;
	tDesc.tColliderDesc.fStaticFriction = 0.5f;
	tDesc.tColliderDesc.fDynamicFriction = 0.5f;
	tDesc.tColliderDesc.fRestitution = 0.6f;
	tDesc.tColliderDesc.pGameObject = this;
	tDesc.vScale = _float3(10.f, 1.f, 10.f);

	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"BoxTrigger", (CComponent**)&m_pBoxCollider,&tDesc)))
		return E_FAIL;
	m_pBoxCollider->Update_PxTransform();
	m_pBoxCollider->setShapeLayer((_uint)ELayer::Trigger);

	return S_OK;
}

void CClient_Trigger::Set_WVPMatrix(void)
{
	_fmatrix matWorld = m_pTransform->Get_WorldMatrix();

	auto matWorldInvers = XMMatrixTranspose(matWorld);
	auto matViewInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	auto matProjInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
}

_fmatrix CClient_Trigger::Get_WorldMat(void)
{
	return m_pTransform->Get_WorldMatrix();
}

void CClient_Trigger::TurnOnTrigger(_bool bCheck)
{
	if (!bCheck)
		m_pBoxCollider->Remove_ActorFromScene();
	else
		m_pBoxCollider->Add_ActorToScene();
}

CGameObject* CClient_Trigger::Clone(const _uint _iSceneID, void* pArg)
{
	CClient_Trigger* pInstance = new CClient_Trigger(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Create CClient_Trigger!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CClient_Trigger* CClient_Trigger::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CClient_Trigger* pInstance = new CClient_Trigger(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CClient_Trigger!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CClient_Trigger::Free(void)
{
	__super::Free();
	Safe_Release(m_pGizmo);
	Safe_Release(m_pBoxCollider);
}
