#include "pch.h"
#include "Trigger.h"
#include "GameInstance.h"
#include "MeshContainer.h"
#include "Gizmo.h"

CTrigger::CTrigger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CTrigger::CTrigger(const CTrigger& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CTrigger::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrigger::NativeConstruct(const _uint _iSceneID, void* pArg)
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

_int CTrigger::Tick(_double TimeDelta)
{
	return _int();
}

_int CTrigger::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);
	return _int();
}

HRESULT CTrigger::Render()
{
	Set_WVPMatrix();

	switch ((TRIGGERTYPE)m_TriggerDesc.eTrigger_Type)
	{
	case TRIGGERTYPE::TRIGGER_LOD:
		m_pGizmo->DrawCapsule(m_pTransform->Get_WorldMatrix(), L"Camera", _fvector{0.129f, 0.65f, 0.929f, 1.0f});
		break;
	case TRIGGERTYPE::TRIGGER_LIGHT:
		m_pGizmo->DrawSphere(m_pTransform->Get_WorldMatrix(), L"Camera", _fvector{ 0.96f, 0.98f, 0.074f, 1.0f });
		break;
	case TRIGGERTYPE::TRIGGER_SCENE:
		m_pGizmo->DrawCube(m_pTransform->Get_WorldMatrix(), L"Camera", _fvector{ 1.0, 0.0f, 0.0f, 1.0f });
		break;
	case TRIGGERTYPE::TRIGGER_MONSTER:
		m_pGizmo->DrawCube(m_pTransform->Get_WorldMatrix(), L"Camera", _fvector{ 0.309f, 0.933f, 0.125f, 1.0f });
		break;
	}
	return S_OK;
}

HRESULT CTrigger::SetUp_Components()
{
	/* Com_Gizmo*/
	m_pGizmo = (CGizmo*)g_pGameInstance->Clone_Component(TAB_STATIC, L"Proto_Component_Gizmo");
	if (nullptr == m_pGizmo)
		return E_FAIL;

	return S_OK;
}

void CTrigger::Set_WVPMatrix(void)
{
	_fmatrix matWorld = m_pTransform->Get_WorldMatrix();
	auto vCamPos = g_pGameInstance->Get_CamPosition(L"Camera");
	 
	auto matWorldInvers = XMMatrixTranspose(matWorld);
	auto matViewInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	auto matProjInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
}

_fmatrix CTrigger::Get_WorldMat(void)
{
	return m_pTransform->Get_WorldMatrix();
}

CGameObject* CTrigger::Clone(const _uint _iSceneID, void* pArg)
{
	CTrigger* pInstance = new CTrigger(*this);

	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Create CTrigger!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CTrigger* CTrigger::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CTrigger* pInstance = new CTrigger(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CTrigger!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrigger::Free(void)
{
	__super::Free();
}
