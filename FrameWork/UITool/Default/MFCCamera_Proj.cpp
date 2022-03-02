#include "framework.h"
#include "pch.h"
#include "MFCCamera_Proj.h"
#include "GameInstance.h"

#include "Mouse.h"

CMFCCamera_Proj::CMFCCamera_Proj(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMFCCamera_Proj::CMFCCamera_Proj(const CMFCCamera_Proj& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMFCCamera_Proj::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMFCCamera_Proj::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	CCamera::CAMERADESC Desc;
	Desc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	Desc.vEye = _float4(0.f, 2.f, -5.f, 1.f);
	Desc.vAt = _float4(0.f, 2.f, 0.f, 1.f);
	Desc.vAxisY = _float4(0.f, 1.f, 0.f, 1.f);

	Desc.fFovy = XMConvertToRadians(65.f);
	Desc.fAspect = _float(WINCX) / _float(WINCY);
	Desc.fNear = 0.01f;
	Desc.fFar = 300.f;

	Desc.pCameraTag = L"MFCCamera_Proj";

	m_pCameraCom = g_pGameInstance->Clone_Component<CCamera>(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Camera", &Desc);

	if (FAILED(SetUp_Components(L"MFCCamera_Proj", m_pCameraCom)))
	{
		return E_FAIL;
	}

	CTransform::TRANSFORMDESC TransDesc;
	TransDesc.fRotationPerSec = XMConvertToRadians(60.0f);
	TransDesc.fSpeedPerSec = 5.f;

	m_pTransform->Set_TransformDesc(TransDesc);

	return S_OK;
}

_int CMFCCamera_Proj::Tick(_double TimeDelta)
{
	if (g_pGameInstance->getkeyPress(DIK_W))
	{
		m_pTransform->Go_Straight(TimeDelta);
	}

	if (g_pGameInstance->getkeyPress(DIK_S))
	{
		m_pTransform->Go_BackWard(TimeDelta);
	}

	if (g_pGameInstance->getkeyPress(DIK_A))
	{
		m_pTransform->Go_Left(TimeDelta);
	}

	if (g_pGameInstance->getkeyPress(DIK_D))
	{
		m_pTransform->Go_Right(TimeDelta);
	}

	if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
	{
		/* 마우스 감도 변수 */
		_long MouseMove = 0;

		_fvector myvec = { 0.f, 1.f, 0.f, 0.f };

		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X))
		{
			m_pTransform->Rotation_Axis(myvec, TimeDelta * MouseMove * 0.1f);
		}

		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_Y))
		{
			m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE::STATE_RIGHT), TimeDelta * MouseMove * 0.1f);
		}
	}

	m_pCameraCom->Update_Matrix(m_pTransform->Get_WorldMatrix());

	return 0;
}

_int CMFCCamera_Proj::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CMFCCamera_Proj::Render()
{
	return S_OK;
}

CMFCCamera_Proj* CMFCCamera_Proj::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMFCCamera_Proj* pInstance = new CMFCCamera_Proj(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMFCCamera_Proj");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMFCCamera_Proj::Clone(void* pArg)
{
	CMFCCamera_Proj* pInstance = new CMFCCamera_Proj(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Clone CMFCCamera_Proj");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMFCCamera_Proj::Free()
{
	Safe_Release(m_pCameraCom);
	__super::Free();
}
