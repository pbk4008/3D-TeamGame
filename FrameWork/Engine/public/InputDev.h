#pragma once
#ifndef __INPUTDEV_H__
#define __INPUTDEV_H__
#include "SingleTon.h"
BEGIN(Engine)
class ENGINE_DLL CInputDev final : public CSingleTon<CInputDev>
{
	friend CSingleTon;
public:
	enum class MOUSESTATE {MB_LBUTTON, MB_RBUTTON,MB_WHILL,MB_END};
	enum class MOUSEMOVESTATE {MM_X, MM_Y,MM_WHILL,MM_END};
private:
	NO_COPY(CInputDev);
	explicit CInputDev();
	virtual ~CInputDev() = default;
public:
	HRESULT Init_InputDevice(HINSTANCE hInstance, HWND hWnd);
	void Update_InputDev();
private:
	virtual void Free();
public:
	_byte getkeyState(_ubyte bykeyID) { return m_byKeyState[bykeyID]; }
	_byte getMouseState(MOUSESTATE eMouse) { return m_tMouseState.rgbButtons[(_uint)eMouse]; }
	_long getMouseMoveState(MOUSEMOVESTATE eMouse) {		return ((_long*)&m_tMouseState)[(_uint)eMouse];	}
	_bool getKeyboardNoKey();
private:
	LPDIRECTINPUT8 m_pInputSDK;
private:
	LPDIRECTINPUTDEVICE8 m_pKeyBoard;
	LPDIRECTINPUTDEVICE8 m_pMouse;
private:
	_byte m_byKeyState[256];
	DIMOUSESTATE m_tMouseState;
};
END
#endif