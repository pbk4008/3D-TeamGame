#include "pch.h"
#include "Level_AnimationTool.h"

CLevel_AnimationTool::CLevel_AnimationTool(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeivceContext)
	: CLevel(_pDevice, _pDeivceContext)
{
}

HRESULT CLevel_AnimationTool::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CLevel_AnimationTool::Tick(_double _dDeltaTime)
{
	return _int();
}

HRESULT CLevel_AnimationTool::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

CLevel_AnimationTool* CLevel_AnimationTool::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CLevel_AnimationTool* pInstance = new CLevel_AnimationTool(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_AnimationTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_AnimationTool::Free()
{

	__super::Free();
}
