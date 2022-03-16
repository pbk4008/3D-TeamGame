#include "pch.h"
#include "Client_Observer.h"

CClient_Observer::CClient_Observer()
{

}

void CClient_Observer::Set_PlayerWordlMat(_fmatrix _matWorld)
{
	XMStoreFloat4x4(&m_matWorld, _matWorld);
}

void CClient_Observer::Free(void)
{
}
