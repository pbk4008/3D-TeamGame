#include "pch.h"
#include "Camera_Culling.h"

#include "Silvermane.h"
#include "Camera_Silvermane.h"

CCamera_Culling::CCamera_Culling(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CCamera_Culling::CCamera_Culling(const CCamera_Culling& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CCamera_Culling::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Culling::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (_pArg)
		m_pCamera_Silvermane = static_cast<CCamera_Silvermane*>(_pArg);

	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

_int CCamera_Culling::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	CTransform* pTargetTransform = m_pCamera_Silvermane->m_pWorldTransform;
	m_pTransform->Set_WorldMatrix(m_pLocalTransform->Get_WorldMatrix() * pTargetTransform->Get_WorldMatrix());
	m_pCamera->Update_Matrix(m_pTransform->Get_WorldMatrix());
	return _int();
}

_int CCamera_Culling::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CCamera_Culling::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCamera_Culling::Ready_Components()
{
	CCamera::CAMERADESC cameraDesc;
	cameraDesc.pCameraTag = L"Camera_Culling";
	cameraDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	cameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	cameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	cameraDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.f);
	cameraDesc.fFovy = XMConvertToRadians(60.f);
	cameraDesc.fAspect = _float(g_iWinCx) / g_iWinCy;
	cameraDesc.fNear = 0.1f;
	cameraDesc.fFar = 300.f;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Camera", L"Com_Camera", (CComponent**)&m_pCamera, &cameraDesc)))
		return E_FAIL;

	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Transform", L"Com_LocalTransform", (CComponent**)&m_pLocalTransform, &transformDesc)))
		return E_FAIL;
	m_pLocalTransform->Set_WorldMatrix(m_pCamera_Silvermane->m_pLocalTransform->Get_WorldMatrix());

	return S_OK;
}

CCamera_Culling* CCamera_Culling::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CCamera_Culling* pInstance = new CCamera_Culling(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCamera_Culling Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCamera_Culling::Clone(const _uint _iSceneID, void* _pArg)
{
	CCamera_Culling* pInstance = new CCamera_Culling(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CCamera_Culling Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Culling::Free()
{
	Safe_Release(m_pLocalTransform);
	Safe_Release(m_pCamera);

	__super::Free();
}