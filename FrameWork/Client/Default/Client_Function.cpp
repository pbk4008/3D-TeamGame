#include "pch.h"
#include "Client_Function.h"

_fvector* MathUtils::GetRandomVector(_fvector* pOut, _fvector* pMin, _fvector* pMax)
{
	XMVectorSetX(*pOut, (_float)ReliableRandom(XMVectorGetX(*pMin), XMVectorGetX(*pMax)));
	XMVectorSetY(*pOut, (_float)ReliableRandom(XMVectorGetY(*pMin), XMVectorGetY(*pMax)));
	XMVectorSetZ(*pOut, (_float)ReliableRandom(XMVectorGetZ(*pMin), XMVectorGetZ(*pMax)));

	return pOut;
}

_double MathUtils::ReliableRandom(_double min, _double max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> distr(min, max);

	return distr(gen);
}

_fvector MathUtils::Direction(_fvector from, _fvector to)
{
	return XMVector3Normalize((to - from));
}

float MathUtils::Length(_fvector a, _fvector b)
{
	return XMVectorGetX(XMVector3Length(a-b));
}

float MathUtils::Length(CGameObject* a, CGameObject* b)
{
	return Length(a->Get_Transform()->Get_State(CTransform::STATE_POSITION), b->Get_Transform()->Get_State(CTransform::STATE_POSITION));
}

float MathUtils::Length(CGameObject* a, CTransform* b)
{
	return Length(a->Get_Transform()->Get_State(CTransform::STATE_POSITION), b->Get_State(CTransform::STATE_POSITION));
}
