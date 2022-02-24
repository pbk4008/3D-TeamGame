#include "framework.h"
#include "pch.h"
#include "MFCCamera.h"
#include "../../Engine/public/GameInstance.h"

CMFCCamera::CMFCCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMFCCamera::CMFCCamera(const CMFCCamera& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMFCCamera::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMFCCamera::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	//_float4 vEye, vAt, vAxisY;
	//_float fAspect, fFovy, fNear, fFar;
	//_float fWinCX, fWinCY;
	//wstring pCameraTag;
	//CCamera::CAMERADESC Desc;

	//Desc.eType = CCamera::CAMERATYPE::CAMERA_ORTHO;

	//Desc.fWinCX = WINCX;
	//Desc.fWinCY = WINCY;

	
	

	return S_OK;
}

_int CMFCCamera::Tick(_double TimeDelta)
{
	Key_Input(TimeDelta);


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CMFCCamera::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CMFCCamera::Render()
{
	return S_OK;
}

void CMFCCamera::Key_Input(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
	}

	/* 마우스 감도 변수 */
	_long MouseMove = 0;

	_fvector myvec = { 0.f, 1.f, 0.f, 0.f };

	if (pGameInstance->Get_DIKeyState(DIK_LCONTROL) & 0x80)
	{
		if (MouseMove = pGameInstance->Get_MouseMoveState(CInput_Device::MMS_X))
		{
		}

		if (MouseMove = pGameInstance->Get_MouseMoveState(CInput_Device::MMS_Y))
		{
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

CMFCCamera* CMFCCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMFCCamera* pInstance = new CMFCCamera(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMFCCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMFCCamera::Clone(void* pArg)
{
	CMFCCamera* pInstance = new CMFCCamera(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Clone CMFCCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMFCCamera::Free()
{
	__super::Free();
}
