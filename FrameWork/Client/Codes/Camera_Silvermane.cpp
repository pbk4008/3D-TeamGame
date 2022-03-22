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

	m_pSilvermane = static_cast<CSilvermane*>(g_pGameInstance->getObjectList(m_iSceneID, L"Layer_Silvermane")->front());
	m_pSilvermane->Set_Camera(this);

	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}

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
	iProgress = LookAt(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	iProgress = Input_Key(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Shaking(_dDeltaTime);
	m_pTransform->Set_WorldMatrix(m_pLocalTransform->Get_WorldMatrix() * m_pWorldTransform->Get_WorldMatrix());
	m_pCamera->Update_Matrix(m_pTransform->Get_WorldMatrix());
	return _int();
}

_int CCamera_Silvermane::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
	{
		return -1;
	}


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
	cameraDesc.fFar = 1000.f;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Camera", L"Com_Camera", (CComponent**)&m_pCamera, &cameraDesc)))
		return E_FAIL;

	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Transform", L"Com_LocalTransform", (CComponent**)&m_pLocalTransform, &transformDesc)))
		return E_FAIL;

	transformDesc.fRotationPerSec = XMConvertToRadians(120.f);
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Transform", L"Com_WorldTransform", (CComponent**)&m_pWorldTransform, &transformDesc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Silvermane::Set_ChaseTarget(const _bool _isChase)
{
	m_isChase = _isChase;
}

void CCamera_Silvermane::Shaking(const _double _dDeltaTime)
{
	//float fOriginDelta = _dDeltaTime;
	if (m_fShakingTime < 0.f)
	{
		m_fShakePower = Lerp(m_fShakePower, 0.f, _dDeltaTime);
		if (fabs(m_fShakePower) <= FLT_EPSILON)
			return;
	}
	
	m_fShakingTime -= _dDeltaTime;
	static int Test = 2;
	Test -= 1;
	if (Test == 0)
	{
		m_fShakePower *= -1.f;
		Test = 2;
	}

	_vector vShake = { 0.001f, 0.0f, 0.001f, 0.0f };
	//auto CamDesc = m_pCamera->Get_CamDesc();
	_vector pos = m_pLocalTransform->Get_State(CTransform::STATE_POSITION);
 	 pos += (vShake * m_fShakePower);
	 pos = XMVectorSetW(pos, 1.0f);

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, pos);

	//XMStoreFloat4((&CamDesc->vAt), XMLoadFloat4(&CamDesc->vAt) + (vShake * m_fShakePower));
}

void CCamera_Silvermane::SetShakeInfo(_float _fShakeTime, _float _fShakePower)
{
	m_fShakingTime = _fShakeTime;
	m_fShakePower = _fShakePower;
}

_int CCamera_Silvermane::Chase_Target(const _double& _dDeltaTime)
{
	if (!m_pSilvermane)
		return - 1;
	if (!m_isChase)
		return 0;

	CTransform* pTargetTransform = m_pSilvermane->Get_Transform();
	_vector svTargetPosition = pTargetTransform->Get_State(CTransform::STATE_POSITION);
	m_pWorldTransform->Set_State(CTransform::STATE_POSITION, svTargetPosition);

	return _int();
}

_int CCamera_Silvermane::LookAt(const _double& _dDeltaTime)
{
	CTransform* pTargetTransform = m_pSilvermane->Get_Transform();

	_vector svTagetRight = XMVector3Normalize(pTargetTransform->Get_State(CTransform::STATE_RIGHT));
	_vector svTagetUp = XMVectorSet(0.f, 1.f, 0.f, 1.f);
	_vector svTagetLook = XMVector3Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK));

	m_vEye = { 0.f, 3.f, -3.f, 1.f };
	_vector svEye = XMLoadFloat4(&m_vEye);
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, svEye);
	
	_vector svAt = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	svAt += svTagetUp;// +XMVectorSet(0.f, 0.f, 0.5f, 0.f);

	XMStoreFloat4(&m_vAt, svAt);

	_vector svLook = svAt - svEye;
	svLook = XMVector3Normalize(svLook);

	_vector svRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), svLook));
	_vector svUp = XMVector3Normalize(XMVector3Cross(svLook, svRight));

	m_pLocalTransform->Set_State(CTransform::STATE_RIGHT, svRight);
	m_pLocalTransform->Set_State(CTransform::STATE_UP, svUp);
	m_pLocalTransform->Set_State(CTransform::STATE_LOOK, svLook);

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
