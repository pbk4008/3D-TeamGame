#include "pch.h"
#include "Dyanmic_Camera.h"
#include "GameInstance.h"

CDynamic_Camera::CDynamic_Camera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
: CGameObject(_pDevice, _pDeviceContext)
{
}

CDynamic_Camera::CDynamic_Camera(const CDynamic_Camera& _rhs)
	:CGameObject(_rhs)
{
}

HRESULT CDynamic_Camera::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL; 

	return S_OK;
}

HRESULT CDynamic_Camera::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	 
	return S_OK;
}

_int CDynamic_Camera::Tick(_double TimeDelta)
{
	m_pCamCom->Update_Matrix(m_pTransform->Get_WorldMatrix());

	return _int();
}

_int CDynamic_Camera::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	Input_Key(TimeDelta);

	return _int();
}

HRESULT CDynamic_Camera::Render()
{
	return S_OK;
}

HRESULT CDynamic_Camera::SetUp_Components()
{
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.pCameraTag = L"Camera";
	CameraDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	CameraDesc.vEye = _float4(0.0f, 0.0f, 0.0f, 1.0f);
	CameraDesc.vAt = _float4(0.0f, 0.0f, 1.0f, 1.0f);
	CameraDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.0f);
	CameraDesc.fFovy = XMConvertToRadians(60.f);
	CameraDesc.fAspect = _float(g_iWINCX) / g_iWINCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CTransform::TRANSFORMDESC TransDesc;

	TransDesc.fRotationPerSec = XMConvertToRadians(120.0f);
	TransDesc.fSpeedPerSec = 30.f;

	if (FAILED(__super::SetUp_Components(TAB_STATIC, L"Camera", L"CamCom", (CComponent**)&m_pCamCom, &CameraDesc)))
		return E_FAIL;

	m_pTransform->Set_TransformDesc(TransDesc);

	return S_OK;
}

void CDynamic_Camera::Input_Key(_double _dTimeDelta)
{

	if (g_pGameInstance->getkeyPress(DIK_W))
	{
		m_pTransform->Go_Straight(_dTimeDelta);
	}

	if (g_pGameInstance->getkeyPress(DIK_S))
	{
		m_pTransform->Go_BackWard(_dTimeDelta);
	}

	if (g_pGameInstance->getkeyPress(DIK_A))
	{
		m_pTransform->Go_Left(_dTimeDelta);
	}

	if (g_pGameInstance->getkeyPress(DIK_D))
	{
		m_pTransform->Go_Right(_dTimeDelta);
	}

	_long	MouseMove = 0;

	if (TKEY_DOWN(VK_SHIFT))
	{
		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X))
		{
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), _dTimeDelta * MouseMove * 0.1f);
		}

		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_Y))
		{
			m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE_RIGHT), _dTimeDelta * MouseMove * 0.1f);
		}
	}
}

CDynamic_Camera* CDynamic_Camera::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CDynamic_Camera* pInstance = new CDynamic_Camera(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CDynamic_Camera!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDynamic_Camera::Clone(void* pArg)
{
	CDynamic_Camera* pInstance = new CDynamic_Camera(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CDynamic_Camera!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDynamic_Camera::Free(void)
{
	__super::Free();

	Safe_Release(m_pCamCom);
}
