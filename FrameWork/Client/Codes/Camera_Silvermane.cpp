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

HRESULT CCamera_Silvermane::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}

	m_pSilvermane = static_cast<CSilvermane*>(g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_TEST_JS, L"Silvermane")->front());

	return S_OK;
}

_int CCamera_Silvermane::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
	{
		return -1;
	}

	if (0 > Chase_Target(_dDeltaTime))
	{
		return -1;
	}

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
	cameraDesc.fFar = 500.f;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Camera", L"Camera", (CComponent**)&m_pCamera, &cameraDesc)))
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
	{
		return - 1;
	}
	if (!m_isChase)
	{
		return 0;
	}
	CTransform* pTargetTransform = m_pSilvermane->Get_Transform();
	
	_vector svTargetRight = pTargetTransform->Get_State(CTransform::STATE_RIGHT);
	_vector svTargetUp = pTargetTransform->Get_State(CTransform::STATE_UP);
	_vector svTargetLook = pTargetTransform->Get_State(CTransform::STATE_LOOK);
	_vector svTargetPosition = pTargetTransform->Get_State(CTransform::STATE_POSITION);


	_vector svX = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	_vector svY = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector svZ = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	_vector svEye = svTargetPosition + svTargetRight + svTargetUp * 3.f + svTargetLook * -4.f;
	_vector svAt = svTargetPosition + svTargetRight + svTargetUp;

	_vector		svLook = svAt - svEye;
	svLook = XMVector3Normalize(svLook);

	_float3 vUp = { 0.f, 1.f, 0.f };
	_vector svRight = XMVector3Cross(XMLoadFloat3(&vUp), svLook);
	svRight = XMVector3Normalize(svRight);

	_vector	svUp = XMVector3Cross(svLook, svRight);
	svUp = XMVector3Normalize(svUp);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, svRight);
	m_pTransform->Set_State(CTransform::STATE_UP, svUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, svLook);
	m_pTransform->Set_State(CTransform::STATE_POSITION, svEye);

	return _int();
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

CGameObject* CCamera_Silvermane::Clone(void* _pArg)
{
	CCamera_Silvermane* pInstance = new CCamera_Silvermane(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CCamera_Silvermane Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Silvermane::Free()
{
	Safe_Release(m_pCamera);

	__super::Free();
}
