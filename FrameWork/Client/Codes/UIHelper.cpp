#include "pch.h"
#include "UIHelper.h"

CUIHelper::CUIHelper(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)

{
}

CUIHelper::CUIHelper(const CUIHelper& rhs)
	: CComponent(rhs)
{
}

HRESULT CUIHelper::NativeConstruct_Prototype()
{
	if (FAILED(CComponent::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIHelper::NativeConstruct(void* pArg)
{
	if (FAILED(CComponent::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CUIHelper::Tick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	return _int();
}

_int CUIHelper::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	return _int();
}

HRESULT CUIHelper::Render(void)
{
	return S_OK;
}

void CUIHelper::UpdateBoundary(CTransform* _TransForm)
{
	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
	_uint iIndex = 1;
	m_pDeviceContext->RSGetViewports(&iIndex, &viewPort);

	_vector vPos = _TransForm->Get_State(CTransform::STATE_POSITION);

	_float fPosX = XMVectorGetX(vPos) + (viewPort.Width * 0.5f);
	_float fPosY = (XMVectorGetY(vPos) * -1) + (viewPort.Height * 0.5f);
	_float fSizX = _TransForm->Get_Scale(CTransform::STATE_RIGHT);
	_float fSizY = _TransForm->Get_Scale(CTransform::STATE_UP);

	SetRect(&m_Boundary, (_uint)(fPosX - fSizX * 0.5f), (_uint)(fPosY - fSizY * 0.5f),
		(_uint)(fPosX + fSizX * 0.5f), (_uint)(fPosY + fSizY * 0.5f));
}

_bool CUIHelper::IsCursorRect(void)
{
	POINT mousePos = getMousePostion();

	if (PtInRect(&m_Boundary, mousePos))
	{
		return true; /* Slot Mouse On */
	}
	return false;
}

POINT CUIHelper::getMousePostion(void)
{
	POINT point = {};

	GetCursorPos(&point);
	ScreenToClient(g_hWnd, &point);

	return point;
}

_bool CUIHelper::MouseClickDown()
{
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	//if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
	{
		if (IsCursorRect())
		{
			return true;
		}
	}
	return false;
}

_bool CUIHelper::MouseClickItem(void)
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	//if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		if (IsCursorRect())
		{
			return true;
		}
	}
	return false;
}

_bool CUIHelper::MouseClickEquipBtn(void)
{
	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		if (IsCursorRect())
		{
			return true;
		}
	}
	return false;
}


_bool CUIHelper::MouseClickArmoryBtn(void)
{
	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		if (IsCursorRect())
		{
			return true;
		}
	}
	return false;
}

CUIHelper* CUIHelper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUIHelper* pInstance = new CUIHelper(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUIHelper Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CUIHelper::Clone(void* pArg)
{
	CUIHelper* pInstance = new CUIHelper(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CUIHelper Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIHelper::Free(void)
{
	__super::Free();
}
