#include "Input_Device.h"

CInput_Device::CInput_Device()
{
}

const _byte CInput_Device::Get_DIKeyState(const _ubyte _byKeyID) const
{
	return m_arrByKeyboardState[_byKeyID];
}

const _long CInput_Device::Get_MouseMoveState(const EMouseMoveState _eMoveState) const
{
	return ((_long*)&m_mouseState)[(_int)_eMoveState];;
}

const _byte CInput_Device::Get_MouseButtonState(const EMouseButtonState _eButtonState) const
{
	return m_mouseState.rgbButtons[(_int)_eButtonState];
}

const _bool CInput_Device::Key_Down(const KEY_STATE & _ks)
{
	return (m_down & _ks) != 0;
}

const _bool CInput_Device::Key_Up(const KEY_STATE & _ks)
{
	return (m_up & _ks) != 0;
}

const _bool CInput_Device::Key_Pressing(const KEY_STATE & _ks)
{
	return (m_press & _ks) != 0;
}

void CInput_Device::ProcessKeyState()
{
	m_press = 0;

	if (m_mouseState.rgbButtons[(_int)EMouseButtonState::LBUTTON])
		m_press |= KEY_LEFT_MOUSE;

	if (m_mouseState.rgbButtons[(_int)EMouseButtonState::RBUTTON])
		m_press |= KEY_RIGHT_MOUSE;

	if (m_mouseState.rgbButtons[(_int)EMouseButtonState::WHEEL])
		m_press |= KEY_WHEEL_MOUSE;

	if (m_arrByKeyboardState[DIK_LEFT])
		m_press |= KEY_LEFT;

	if (m_arrByKeyboardState[DIK_RIGHT])
		m_press |= KEY_RIGHT;

	if (m_arrByKeyboardState[DIK_UP])
		m_press |= KEY_UP;

	if (m_arrByKeyboardState[DIK_DOWN])
		m_press |= KEY_DOWN;

	if (m_arrByKeyboardState[DIK_SPACE])
		m_press |= KEY_SPACE;

	if (m_arrByKeyboardState[DIK_RETURN])
		m_press |= KEY_ENTER;

	if (m_arrByKeyboardState[DIK_TAB])
		m_press |= KEY_TAB;

	if (m_arrByKeyboardState[DIK_LCONTROL])
		m_press |= KEY_CONTROL;

	if (m_arrByKeyboardState[DIK_RCONTROL])
		m_press |= KEY_CONTROL;

	if (m_arrByKeyboardState[DIK_BACKSPACE])
		m_press |= KEY_BACKSPACE;

	if (m_arrByKeyboardState[DIK_ESCAPE])
		m_press |= KEY_ESCAPE;

	if (m_arrByKeyboardState[DIK_LALT])
		m_press |= KEY_ALT;

	if (m_arrByKeyboardState[DIK_RALT])
		m_press |= KEY_ALT;

	if (m_arrByKeyboardState[DIK_F1])
		m_press |= KEY_F1;

	if (m_arrByKeyboardState[DIK_F2])
		m_press |= KEY_F2;

	if (m_arrByKeyboardState[DIK_F3])
		m_press |= KEY_F3;

	if (m_arrByKeyboardState[DIK_F4])
		m_press |= KEY_F4;

	if (m_arrByKeyboardState[DIK_F5])
		m_press |= KEY_F5;

	if (m_arrByKeyboardState[DIK_F6])
		m_press |= KEY_F6;

	if (m_arrByKeyboardState[DIK_F7])
		m_press |= KEY_F7;

	if (m_arrByKeyboardState[DIK_F8])
		m_press |= KEY_F8;

	if (m_arrByKeyboardState[DIK_F9])
		m_press |= KEY_F9;

	if (m_arrByKeyboardState[DIK_F10])
		m_press |= KEY_F10;

	if (m_arrByKeyboardState[DIK_F11])
		m_press |= KEY_F11;

	if (m_arrByKeyboardState[DIK_F12])
		m_press |= KEY_F12;

	if (m_arrByKeyboardState[DIK_LSHIFT])
		m_press |= KEY_SHIFT;

	if (m_arrByKeyboardState[DIK_A])
		m_press |= KEY_A;

	if (m_arrByKeyboardState[DIK_B])
		m_press |= KEY_B;

	if (m_arrByKeyboardState[DIK_C])
		m_press |= KEY_C;

	if (m_arrByKeyboardState[DIK_D])
		m_press |= KEY_D;

	if (m_arrByKeyboardState[DIK_E])
		m_press |= KEY_E;

	if (m_arrByKeyboardState[DIK_F])
		m_press |= KEY_F;

	if (m_arrByKeyboardState[DIK_G])
		m_press |= KEY_G;

	if (m_arrByKeyboardState[DIK_H])
		m_press |= KEY_H;

	if (m_arrByKeyboardState[DIK_I])
		m_press |= KEY_I;

	if (m_arrByKeyboardState[DIK_J])
		m_press |= KEY_J;

	if (m_arrByKeyboardState[DIK_K])
		m_press |= KEY_K;

	if (m_arrByKeyboardState[DIK_L])
		m_press |= KEY_L;

	if (m_arrByKeyboardState[DIK_M])
		m_press |= KEY_M;

	if (m_arrByKeyboardState[DIK_N])
		m_press |= KEY_N;

	if (m_arrByKeyboardState[DIK_O])
		m_press |= KEY_O;

	if (m_arrByKeyboardState[DIK_P])
		m_press |= KEY_P;

	if (m_arrByKeyboardState[DIK_Q])
		m_press |= KEY_Q;

	if (m_arrByKeyboardState[DIK_R])
		m_press |= KEY_R;

	if (m_arrByKeyboardState[DIK_S])
		m_press |= KEY_S;

	if (m_arrByKeyboardState[DIK_T])
		m_press |= KEY_T;

	if (m_arrByKeyboardState[DIK_U])
		m_press |= KEY_U;

	if (m_arrByKeyboardState[DIK_V])
		m_press |= KEY_V;

	if (m_arrByKeyboardState[DIK_W])
		m_press |= KEY_W;

	if (m_arrByKeyboardState[DIK_X])
		m_press |= KEY_X;

	if (m_arrByKeyboardState[DIK_Y])
		m_press |= KEY_Y;

	if (m_arrByKeyboardState[DIK_Z])
		m_press |= KEY_Z;

	if (m_arrByKeyboardState[DIK_0])
		m_press |= KEY_0;

	if (m_arrByKeyboardState[DIK_1])
		m_press |= KEY_1;

	if (m_arrByKeyboardState[DIK_2])
		m_press |= KEY_2;

	if (m_arrByKeyboardState[DIK_3])
		m_press |= KEY_3;

	if (m_arrByKeyboardState[DIK_4])
		m_press |= KEY_4;

	if (m_arrByKeyboardState[DIK_5])
		m_press |= KEY_5;

	if (m_arrByKeyboardState[DIK_6])
		m_press |= KEY_6;

	if (m_arrByKeyboardState[DIK_7])
		m_press |= KEY_7;

	if (m_arrByKeyboardState[DIK_8])
		m_press |= KEY_8;

	if (m_arrByKeyboardState[DIK_9])
		m_press |= KEY_9;

	m_changed = (m_pressed^m_press);		// 0101 ^ 0011 = 0110	// 변화된 부위를 나타냄 
	m_down = (m_press & m_changed);			// 0011 & 0110	= 0010			// 1로 변화된 곳은 Down 스위치를 켜기
	m_up = ((~m_press)&m_changed);			// 1100 & 0110	= 0100		// 0으로 변화된 곳은 Up 스위치를 켜기
	m_pressed = m_press;					// 다음 프레임 준비
}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE _hInst, HWND _hWnd)
{
	if (FAILED(DirectInput8Create(_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
	{
		return E_FAIL;
	}

	/* 키보드 초기화. */
 	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
	{
		return E_FAIL;
	}

	m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);

	if (FAILED(m_pKeyboard->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE /*| DISCL_NOWINKEY*/)))
		return E_FAIL;

	m_pKeyboard->Acquire();

	/* 마우스 초기화. */
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
	{
		return E_FAIL;
	}

	m_pMouse->SetDataFormat(&c_dfDIMouse);

	if(FAILED(m_pMouse->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE /*| DISCL_NOWINKEY*/)))
		return E_FAIL;

	m_pMouse->Acquire();

	return S_OK;
}

HRESULT CInput_Device::SetUp_InputDeviceState()
{
	if (WA_INACTIVE == GetActiveWindow())
	{
		return S_OK;
	}

	if (nullptr == m_pKeyboard ||
		nullptr == m_pMouse)
	{
		return E_FAIL;
	}


	if (FAILED(m_pKeyboard->GetDeviceState(256, m_arrByKeyboardState)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState)))
	{
		return E_FAIL;
	}

	ProcessKeyState();

	return S_OK;
}

void CInput_Device::Free()
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyboard);
	Safe_Release(m_pInputSDK);
}
