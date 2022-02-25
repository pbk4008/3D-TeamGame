#pragma once

#include "SingleTon.h"

typedef unsigned long long KEY_STATE;

BEGIN(Engine)

class CInput_Device final : public CSingleTon<CInput_Device>
{
	friend CSingleTon;
public:
	enum class EMouseMoveState { X, Y, WHEEL, MAX };
	enum class EMouseButtonState { LBUTTON, RBUTTON, WHEEL, MAX };;
private:
	explicit CInput_Device();
	virtual ~CInput_Device() = default;
public:
	const _byte Get_DIKeyState(const _ubyte _byKeyID) const;
	const _long Get_MouseMoveState(const EMouseMoveState _eMoveState) const;
	const _byte Get_MouseButtonState(const EMouseButtonState _eButtonState) const;

public:
	const _bool Key_Down(const KEY_STATE& _ks);
	const _bool Key_Up(const KEY_STATE& _ks);
	const _bool Key_Pressing(const KEY_STATE& _ks);

private:
	void ProcessKeyState();

public:
	HRESULT Ready_Input_Device(HINSTANCE _hInst, HWND _hWnd);
	HRESULT SetUp_InputDeviceState();
private:
	IDirectInput8* m_pInputSDK = nullptr;
	IDirectInputDevice8* m_pKeyboard = nullptr;
	IDirectInputDevice8* m_pMouse = nullptr;

private:
	_byte m_arrByKeyboardState[256];
	DIMOUSESTATE m_mouseState;

	_ulong m_changed = 0;
	_ulong m_press = 0;
	_ulong m_pressed = 0;
	_ulong m_up = 0;
	_ulong m_down = 0;
	
public:
	virtual void Free() override;
};

END

#define KEY_DEFAULT	0x0000000000000000

#define KEY_LEFT		0x0000000000000001
#define KEY_RIGHT		0x0000000000000002
#define KEY_DOWN		0x0000000000000004
#define KEY_UP			0x0000000000000008

#define KEY_LEFT_MOUSE	0x0000000000000010
#define KEY_RIGHT_MOUSE	0x0000000000000020
#define KEY_SPACE		0x0000000000000040
#define KEY_ENTER		0x0000000000000080

#define KEY_TAB			0x0000000000000100
#define KEY_SHIFT		0x0000000000000200
#define KEY_CONTROL		0x0000000000000400
#define KEY_BACKSPACE	0x0000000000000800

#define KEY_ESCAPE		0x0000000000001000
#define KEY_ALT			0x0000000000002000
#define KEY_WHEEL_MOUSE 0x0000000000004000
//#define KEY_0			0x0000000000008000

#define KEY_A			0x0000000000010000
#define KEY_B			0x0000000000020000
#define KEY_C			0x0000000000040000
#define KEY_D			0x0000000000080000

#define KEY_E			0x0000000000100000
#define KEY_F			0x0000000000200000
#define KEY_G			0x0000000000400000
#define KEY_H			0x0000000000800000

#define KEY_I			0x0000000001000000
#define KEY_J			0x0000000002000000
#define KEY_K			0x0000000004000000
#define KEY_L			0x0000000008000000

#define KEY_M			0x0000000010000000
#define KEY_N			0x0000000020000000
#define KEY_O			0x0000000040000000
#define KEY_P			0x0000000080000000

#define KEY_Q			0x0000000100000000
#define KEY_R			0x0000000200000000
#define KEY_S			0x0000000400000000
#define KEY_T			0x0000000800000000

#define KEY_U			0x0000001000000000
#define KEY_V			0x0000002000000000
#define KEY_W			0x0000004000000000
#define KEY_X			0x0000008000000000

#define KEY_Y			0x0000010000000000
#define KEY_Z			0x0000020000000000
#define KEY_1			0x0000040000000000
#define KEY_2			0x0000080000000000

#define KEY_3			0x0000100000000000
#define KEY_4			0x0000200000000000
#define KEY_5			0x0000400000000000
#define KEY_6			0x0000800000000000

#define KEY_7			0x0001000000000000
#define KEY_8			0x0002000000000000
#define KEY_9			0x0004000000000000
#define KEY_0			0x0008000000000000

#define KEY_F1			0x0010000000000000
#define KEY_F2			0x0020000000000000
#define KEY_F3			0x0040000000000000
#define KEY_F4			0x0080000000000000

#define KEY_F5			0x0100000000000000
#define KEY_F6			0x0200000000000000
#define KEY_F7			0x0400000000000000
#define KEY_F8			0x0800000000000000

#define KEY_F9				0x1000000000000000
#define KEY_F10			0x2000000000000000
#define KEY_F11			0x4000000000000000
#define KEY_F12			0x8000000000000000

#define KEY_ALL			0xffffffffffffffff