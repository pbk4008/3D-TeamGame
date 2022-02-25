#include "InputDev.h"

CInputDev::CInputDev() 
	: m_pInputSDK(nullptr)
	,m_pKeyBoard(nullptr)
	,m_pMouse(nullptr)
{
	ZeroMemory(m_byKeyState, sizeof(_byte) * 256);
	ZeroMemory(&m_tMouseState, sizeof(DIMOUSESTATE));
}

HRESULT CInputDev::Init_InputDevice(HINSTANCE hInstance, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	m_pKeyBoard->Acquire();


	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse);

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	m_pMouse->Acquire();

	return S_OK;
}

void CInputDev::Update_InputDev()
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}
void CInputDev::Free()
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

_bool CInputDev::getKeyboardNoKey()
{
	for (_uint i = 0; i < 256; i++)
	{
		_byte keyValue = getkeyState(i);
		if (keyValue)
			return false;
	}
	return true;
}

