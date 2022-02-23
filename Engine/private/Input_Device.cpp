#include "..\public\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{
}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;
	
	/* Ű���� �ʱ�ȭ. */
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE/* | DISCL_NOWINKEY*/)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->Acquire()))
		return E_FAIL;

	/* ���콺 �ʱ�ȭ. */
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE/* | DISCL_NOWINKEY*/)))
		return E_FAIL;

	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInput_Device::SetUp_InputDeviceState()
{
	if (nullptr == m_pKeyBoard ||
		nullptr == m_pMouse)
		return E_FAIL;


	if (FAILED(m_pKeyBoard->GetDeviceState(256, m_KeyBoardState)))
		return E_FAIL;

	if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)))
		return E_FAIL;

	return S_OK;
}

void CInput_Device::Free()
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pInputSDK);
}
