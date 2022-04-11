#pragma once
#ifndef __CLIENT_FUNCTION_H__
#define __CLIENT_FUNCTION_H__

#include <random>

BEGIN(Client)

class MathUtils
{
public:
	static _fvector* GetRandomVector(_Out_ _fvector* pOut, _fvector* pMin, _fvector* pMax);
	static _double ReliableRandom(_double min, _double max);
	static _fvector Direction(_fvector from, _fvector to);
	static float Length(_fvector a, _fvector b);
	static float Length(CGameObject* a, CGameObject* b);

};

//static _fvector getMousePostion()
//{
//	POINT point = {};
//	GetCursorPos(&point);
//	ScreenToClient(g_hWnd, &point);
//
//	return _fvector{ (_float)point.x,(_float)point.y, 1.f, 1.f };
//};

END
#endif