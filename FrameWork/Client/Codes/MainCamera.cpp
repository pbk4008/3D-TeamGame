#include "pch.h"
#include "MainCamera.h"

CMainCamera::CMainCamera()
	: m_pCamera(nullptr)
{
}

CMainCamera::CMainCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pCamera(nullptr)
{
}

CMainCamera::CMainCamera(const CMainCamera& rhs)
	: CGameObject(rhs)
	, m_pCamera(rhs.m_pCamera)
{
	Safe_AddRef(m_pCamera);
}

HRESULT CMainCamera::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CMainCamera::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_GameObject(pArg)))
		return E_FAIL;

	m_pTransform->Set_TransformDesc(15.f, XMConvertToRadians(30.f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, -1.f, 1.f));
	//m_pTransform->SetUp_Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(30.f));
	return S_OK;
}

_int CMainCamera::Tick(_double fDeltaTime)
{
	if (g_pGameInstance->getkeyPress(DIK_W))
	{
		m_pTransform->Go_Straight(fDeltaTime);
	}

	if (g_pGameInstance->getkeyPress(DIK_S))
	{
		m_pTransform->Go_BackWard(fDeltaTime);
	}

	if (g_pGameInstance->getkeyPress(DIK_A))
	{
		m_pTransform->Go_Left(fDeltaTime);
	}

	if (g_pGameInstance->getkeyPress(DIK_D))
	{
		m_pTransform->Go_Right(fDeltaTime);
	}

	/* 마우스 감도 변수 */
	_long MouseMove = 0;

	_fvector myvec = { 0.f, 1.f, 0.f, 0.f };

	if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X))
	{
		m_pTransform->Rotation_Axis(myvec, fDeltaTime * MouseMove * 0.1f);
	}

	if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_Y))
	{
		m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE::STATE_RIGHT), fDeltaTime * MouseMove * 0.1f);
	}

	m_pCamera->Update_Matrix(m_pTransform->Get_WorldMatrix());

	return _int();
}

_int CMainCamera::LateTick(_double fDeltaTime)
{
	return _int();
}

HRESULT CMainCamera::Render()
{
	return S_OK;
}

CGameObject* CMainCamera::Clone(const _uint _iSceneID, void* pArg)
{
	CMainCamera* pInstance = new CMainCamera(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("CMainCamera Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CMainCamera* CMainCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMainCamera* pInstance = new CMainCamera(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMainCamera Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

HRESULT CMainCamera::Ready_GameObject(void* pArg)
{
	CCamera::CAMERADESC tDesc = (*(CCamera::CAMERADESC*)pArg);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Camera", L"Com_MainCameara", (CComponent * *)& m_pCamera, &tDesc)))
		return E_FAIL;

	return S_OK;
}

void CMainCamera::Free()
{
	CGameObject::Free();
	Safe_Release(m_pCamera);
}
