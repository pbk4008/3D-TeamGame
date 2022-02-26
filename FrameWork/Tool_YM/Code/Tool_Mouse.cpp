#include "pch.h"
#include "Tool_Mouse.h"
#include "GameInstance.h"

CTool_Mouse::CTool_Mouse()
{
}

HRESULT CTool_Mouse::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTool_Mouse::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CTool_Mouse::Tick(_double TimeDelta)
{
	return _int();
}

CTool_Mouse* CTool_Mouse::Create()
{
	CTool_Mouse* pInstance = new CTool_Mouse();

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CTool_Mouse!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTool_Mouse::Clone(void* pArg)
{
	CTool_Mouse* pInstance = new CTool_Mouse(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CTool_Mouse!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTool_Mouse::Free(void)
{
	__super::Free();
}
