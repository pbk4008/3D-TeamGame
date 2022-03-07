#include "pch.h"
#include "Camera_Debug.h"

CCamera_Debug::CCamera_Debug(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
: CGameObject(_pDevice, _pDeviceContext)
{
}

CCamera_Debug::CCamera_Debug(const CCamera_Debug& _rhs)
	:CGameObject(_rhs)
{
}

HRESULT CCamera_Debug::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL; 

	return S_OK;
}

HRESULT CCamera_Debug::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	 
	return S_OK;
}

_int CCamera_Debug::Tick(_double TimeDelta)
{
	m_pCamera->Update_Matrix(m_pTransform->Get_WorldMatrix());

	return _int();
}

_int CCamera_Debug::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	Input_Key(TimeDelta);

	return _int();
}

HRESULT CCamera_Debug::Render()
{
	return S_OK;
}

HRESULT CCamera_Debug::SetUp_Components()
{
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.pCameraTag = L"Camera_Debug";
	CameraDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	CameraDesc.vEye = _float4(0.0f, 0.0f, 0.0f, 1.0f);
	CameraDesc.vAt = _float4(0.0f, 0.0f, 1.0f, 1.0f);
	CameraDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.0f);
	CameraDesc.fFovy = XMConvertToRadians(60.f);
	CameraDesc.fAspect = _float(g_iWinCx) / g_iWinCy;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CTransform::TRANSFORMDESC TransDesc;

	TransDesc.fRotationPerSec = XMConvertToRadians(120.0f);
	TransDesc.fSpeedPerSec = 10.f;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Camera", L"Camera", (CComponent**)&m_pCamera, &CameraDesc)))
		return E_FAIL;

	m_pTransform->Set_TransformDesc(TransDesc);

	return S_OK;
}

void CCamera_Debug::Input_Key(_double _dTimeDelta)
{
	if (g_pGameInstance->getMousePress(CInputDev::MOUSESTATE::MB_RBUTTON))
	{
		if (g_pGameInstance->getkeyPress(DIK_UP))
			m_pTransform->Go_Straight(_dTimeDelta);
		if (g_pGameInstance->getkeyPress(DIK_DOWN))
			m_pTransform->Go_BackWard(_dTimeDelta);
		if (g_pGameInstance->getkeyPress(DIK_LEFT))
			m_pTransform->Go_Left(_dTimeDelta);
		if (g_pGameInstance->getkeyPress(DIK_RIGHT))
			m_pTransform->Go_Right(_dTimeDelta);

		_long	MouseMove = 0;
		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X))
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), _dTimeDelta * MouseMove * 0.1f);
		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_Y))
			m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE_RIGHT), _dTimeDelta * MouseMove * 0.1f);
	}
}

CCamera_Debug* CCamera_Debug::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CCamera_Debug* pInstance = new CCamera_Debug(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CCamera_Debug!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCamera_Debug::Clone(void* pArg)
{
	CCamera_Debug* pInstance = new CCamera_Debug(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CCamera_Debug!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Debug::Free(void)
{
	Safe_Release(m_pCamera);

	__super::Free();
}
