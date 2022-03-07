#include "pch.h"
#include "ToolCamera.h"
#include "GameInstance.h"

CToolCamera::CToolCamera()
	: m_pCamera(nullptr)
{
}

CToolCamera::CToolCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pCamera(nullptr)
{

}

CToolCamera::CToolCamera(const CToolCamera& rhs)
	:CGameObject(rhs)
	, m_pCamera(nullptr)
{
}

HRESULT CToolCamera::Init_GameObject(COMINDEX* pIndex)
{
	if (FAILED(CGameObject::Init_GameObject(pIndex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolCamera::Init_Clone(void* pArg)
{
	CTransform::TRANSDESC pDesc;
	ZeroMemory(&pDesc, sizeof(CTransform::TRANSDESC));

	pDesc.fRotatePerSec = XMConvertToRadians(10.f);
	pDesc.fSpeedPerSec = 10.f;

	if (pArg)
	{
		CGameObject::COMINDEX* pIndex = (CGameObject::COMINDEX*)pArg;
		for (_uint i = 0; i < (_uint)COMPONENTID::COMPONENT_END; i++)
		{
			if (pIndex[i].iStartIndex == -1)
				continue;

			m_pComponentIndex[i] = pIndex[i];
		}
	}
	if (FAILED(CGameObject::Init_Clone(&pDesc)))
		return E_FAIL;

	if (FAILED(Add_Component<CCamera>((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::CAMERA, m_pComponentIndex[(_uint)COMPONENTID::CAMERA].iStartIndex, &m_pCamera)))
		return E_FAIL;

	if (m_pCamera->getType())
	{
		m_pTransform->setState(CTransform::TRANS_STATE::STATE_POS, XMVectorSet(0.f, 10.f, -5.f, 1.f));
		m_pTransform->Set_Rotate(XMVectorSet(1.0f, 0.f, 0.f, 0.f), XMConvertToRadians(30.f));
	}


	return S_OK;
}

_int CToolCamera::Update_GameObject(_float fDeltaTime)
{
	if (0 > (CGameObject::Update_GameObject(fDeltaTime)))
		return -1;

	if (m_pCamera->getType())
		CameraMove(fDeltaTime);

	m_pCamera->Update_Matrix(m_pTransform->getWorldMatrix());
	return 0;
}

HRESULT CToolCamera::Render_GameObject()
{
	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	return S_OK;
}

void CToolCamera::CameraMove(_float fDeltaTime)
{
	if (!m_pTransform)
		return;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	if (pInstance->GetkeyState(DIK_W))
		m_pTransform->Go_Forward(fDeltaTime);
	if (pInstance->GetkeyState(DIK_A))
		m_pTransform->Go_Left(fDeltaTime);
	if (pInstance->GetkeyState(DIK_S))
		m_pTransform->Go_BackWard(fDeltaTime);
	if (pInstance->GetkeyState(DIK_D))
		m_pTransform->Go_Right(fDeltaTime);

	_long MouseMove = 0;

	if (pInstance->GetMouseState(CInputDev::MOUSESTATE::MB_RBUTTON))
	{
		if (MouseMove = pInstance->GetMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X))
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime * MouseMove * 0.2f);
		if (MouseMove = pInstance->GetMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_Y))
			m_pTransform->Rotation_Axis(m_pTransform->getState(CTransform::TRANS_STATE::STATE_RIGHT), fDeltaTime * MouseMove * 0.2f);
	}
	RELEASE_INSTANCE(CGameInstance);
}

CToolCamera* CToolCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, COMINDEX* pIndex)
{
	CToolCamera* pInstance = new CToolCamera(pDevice, pDeviceContext);
	if (FAILED(pInstance->Init_GameObject(pIndex)))
	{
		MSG_BOX(L"ToolMainCamera Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CToolCamera::Clone(void* pArg)
{
	CToolCamera* pInstance = new CToolCamera(*this);
	if (FAILED(pInstance->Init_Clone(pArg)))
	{
		MSG_BOX(L"ToolMainCamera Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CToolCamera::Free()
{
	CGameObject::Free();
	Safe_Release(m_pCamera);
}
