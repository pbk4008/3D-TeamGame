#include "pch.h"
#include "MaterialCamera.h"

CMaterialCamera::CMaterialCamera()
	: m_pCamera(nullptr)
{
}

CMaterialCamera::CMaterialCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pCamera(nullptr)
{
}

CMaterialCamera::CMaterialCamera(const CMaterialCamera& rhs)
	: CGameObject(rhs)
	, m_pCamera(rhs.m_pCamera)
{
	Safe_AddRef(m_pCamera);
}

HRESULT CMaterialCamera::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMaterialCamera::NativeConstruct(void* pArg)
{
	if (!pArg)
		return S_OK;

	if (FAILED(CGameObject::NativeConstruct(pArg)))
		return E_FAIL;
	CCamera::CAMERADESC tDesc = (*(CCamera::CAMERADESC*)pArg);
	if (FAILED(SetUp_Components(0, L"Camera", L"MaterialCamera", (CComponent * *)& m_pCamera, &tDesc)))
		return E_FAIL;
	m_pTransform->SetTransformDesc(3.f, XMConvertToRadians(30.f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 30.f, 10.f,1.f));
	return S_OK;
}

_int CMaterialCamera::Tick(_double dDeltaTime)
{
	if (!m_pCamera)
		return -1;

	_long   MouseMove = 0;

	if (g_pGameInstance->getkeyPress(DIK_LSHIFT))
	{
		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X))
		{
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), dDeltaTime * MouseMove * 0.1f);
		}

		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_Y))
		{
			m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE_RIGHT), dDeltaTime * MouseMove * 0.1f);
		}
	}

	if (g_pGameInstance->getkeyPress(DIK_D))
		m_pTransform->Go_Right(dDeltaTime);
	if (g_pGameInstance->getkeyPress(DIK_W))
		m_pTransform->Go_Straight(dDeltaTime);
	if (g_pGameInstance->getkeyPress(DIK_A))
		m_pTransform->Go_Left(dDeltaTime);
	if (g_pGameInstance->getkeyPress(DIK_S))
		m_pTransform->Go_BackWard(dDeltaTime);


	m_pCamera->Update_Matrix(m_pTransform->Get_WorldMatrix());
	return _int();
}

CMaterialCamera* CMaterialCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMaterialCamera* pInstance = new CMaterialCamera(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMaterialCamera Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMaterialCamera::Clone(void* pArg)
{
	CMaterialCamera* pInstance = new CMaterialCamera(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMaterialCamera Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMaterialCamera::Free()
{
	CGameObject::Free();
	Safe_Release(m_pCamera);
}
