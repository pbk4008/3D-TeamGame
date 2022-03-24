#include "pch.h"
#include "Camera_Silvermane.h"

#include "Silvermane.h"

CCamera_Silvermane::CCamera_Silvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CCamera_Silvermane::CCamera_Silvermane(const CCamera_Silvermane& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CCamera_Silvermane::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCamera_Silvermane::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}

	m_pSilvermane = static_cast<CSilvermane*>(g_pGameInstance->getObjectList(m_iSceneID, L"Layer_Silvermane")->front());
	m_pSilvermane->Set_Camera(this);

	return S_OK;
}

_int CCamera_Silvermane::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	iProgress = Chase_Target(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	iProgress = Input_Key(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pTransform->Set_WorldMatrix(m_pLocalTransform->Get_WorldMatrix() * m_pWorldTransform->Get_WorldMatrix());

	return _int();
}

_int CCamera_Silvermane::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pCamera->Update_Matrix(m_pTransform->Get_WorldMatrix());

	return _int();
}

HRESULT CCamera_Silvermane::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCamera_Silvermane::Ready_Components()
{
	CCamera::CAMERADESC cameraDesc;
	cameraDesc.pCameraTag = L"Camera_Silvermane";
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
	_float4 vPosition = { 0.5f, 3.f, -3.f, 1.f };
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));
	m_pLocalTransform->SetUp_Rotation(m_pLocalTransform->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(30.f));

	transformDesc.fRotationPerSec = XMConvertToRadians(120.f);
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Transform", L"Com_WorldTransform", (CComponent**)&m_pWorldTransform, &transformDesc)))
		return E_FAIL;


	return S_OK;
}

void CCamera_Silvermane::Set_ChaseTarget(const _bool _isChase)
{
	m_isChase = _isChase;
}

_int CCamera_Silvermane::Chase_Target(const _double& _dDeltaTime)
{
	if (!m_pSilvermane)
		return -1;
	if (!m_isChase)
		return 0;

	CTransform* pTargetTransform = m_pSilvermane->Get_Transform();
	_vector svTargetPosition = pTargetTransform->Get_State(CTransform::STATE_POSITION);
	m_pWorldTransform->Set_State(CTransform::STATE_POSITION, svTargetPosition);

	return _int();
}

_int CCamera_Silvermane::Input_Key(const _double& _dDeltaTime)
{
	_long   MouseMove = 0;

	if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X))
		m_pWorldTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), _dDeltaTime * MouseMove * 0.1f);

	MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_Y);
	m_fRotRight += (_float)_dDeltaTime * MouseMove;
	m_pWorldTransform->Rotation_Axis(m_pWorldTransform->Get_State(CTransform::STATE_RIGHT), _dDeltaTime * MouseMove * 0.1f);

	return _int();
}

const _fvector& CCamera_Silvermane::Get_Look() const
{
	return m_pWorldTransform->Get_State(CTransform::STATE_LOOK);
}

CCamera_Silvermane* CCamera_Silvermane::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CCamera_Silvermane* pInstance = new CCamera_Silvermane(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCamera_Silvermane Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCamera_Silvermane::Clone(const _uint _iSceneID, void* _pArg)
{
	CCamera_Silvermane* pInstance = new CCamera_Silvermane(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CCamera_Silvermane Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Silvermane::Free()
{
	Safe_Release(m_pLocalTransform);
	Safe_Release(m_pWorldTransform);

	Safe_Release(m_pCamera);

	__super::Free();
}