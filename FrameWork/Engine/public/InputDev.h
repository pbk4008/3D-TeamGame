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
	_bool getkeyPress(_ubyte bykeyID);
	_bool getkeyDown(_ubyte bykeyID);
	_bool getkeyUp(_ubyte bykeyID);
	_bool getMousePress(MOUSESTATE eMouse);
	_bool getMouseKeyDown(MOUSESTATE eMouse);
	_bool getMouseKeyUp(MOUSESTATE eMouse);

	_long getMouseMoveState(MOUSEMOVESTATE eMouse) {		return ((_long*)&m_tMouseState)[(_uint)eMouse];	}
	_bool getKeyboardNoKey();
private:
	LPDIRECTINPUT8 m_pInputSDK;
private:
	LPDIRECTINPUTDEVICE8 m_pKeyBoard;
	LPDIRECTINPUTDEVICE8 m_pMouse;
private:
	_byte m_byKeyState[256];
	_byte m_byKeyDown[256];
	_byte m_byKeyUp[256];
	_byte m_byMouseKeyUp[(_uint)MOUSESTATE::MB_END];
	_byte m_byMouseKeyDown[(_uint)MOUSESTATE::MB_END];
	DIMOUSESTATE m_tMouseState;
};
END
#endif