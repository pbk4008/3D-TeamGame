#include "pch.h"
#include "Cinema1_1.h"
#include "CinemaCam.h"

CCinema1_1::CCinema1_1()
	: m_pCam(nullptr)
	, m_fAccTest(0.f)
{
}

CCinema1_1::CCinema1_1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_fAccTest(0.f)
{
}

HRESULT CCinema1_1::NativeContruct()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

_int CCinema1_1::Tick(_double dDeltaTime)
{
	_uint iProgress=CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	m_pCam->Tick(dDeltaTime);

	m_fAccTest += dDeltaTime;
	if (m_fAccTest > 5.f)
		m_bCinemaEnd = true;

	return _int();
}

_int CCinema1_1::LateTick(_double dDeltaTime)
{
	return _int();
}

void CCinema1_1::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);

	if (m_bActive)
		m_pCam->Change_CurrentCam();
}

HRESULT CCinema1_1::Ready_Components()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"Cienema1_1";
	tDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	tDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	tDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.f);
	tDesc.fFovy = XMConvertToRadians(90.f);
	tDesc.fFar = 300.f;
	tDesc.fNear = 0.1f;
	tDesc.fAspect = (_float)g_iWinCx / g_iWinCy;

	m_pCam=g_pGameInstance->Clone_GameObject<CCinemaCam>((_uint)SCENEID::SCENE_STAGE1, L"Proto_GameObject_CinemaCamera", &tDesc);
	if (!m_pCam)
	{
		MSGBOX("Came Create Fail");
		return E_FAIL;
	}

	return S_OK;
}

CCinema1_1* CCinema1_1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{

	CCinema1_1* pInstance = new CCinema1_1(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeContruct()))
	{
		MSGBOX("Cinema1_1 Crate Faile");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCinema1_1::Free()
{
	CScenematic::Free();
	Safe_Release(m_pCam);
}
