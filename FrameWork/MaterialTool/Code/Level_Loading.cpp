#include "pch.h"
#include "Level_Loading.h"

#include "Loader.h"

#include "Level_AnimationTool.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeivceContext)
	: CLevel(_pDevice, _pDeivceContext)
{
}

HRESULT CLevel_Loading::NativeConstruct(const ELevel _eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	m_eNextLevel = _eNextLevel;

	/* 다음레벨에 필요한 ㅎ원형객체들을 새엇앟낟. */
	m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, m_eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

_int CLevel_Loading::Tick(_double _dDeltaTime)
{
	if (nullptr == m_pLoader)
		return -1;

	if (0 > (__super::Tick(_dDeltaTime)))
		return -1;

	/* 로더야 다했냐? */
	if (true == m_pLoader->Get_Finished())
	{
		if (FAILED(Open_Level()))
			return -1;
		return 0;
	}

	return _int();
}

HRESULT CLevel_Loading::Render()
{
	if (!m_pLoader)
		return E_FAIL;

	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	//m_pLoader->Print_LoaderText();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const wstring& _wstrLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.GameObject_BackGround */
	if (FAILED(pGameInstance->Add_GameObjectToLayer((_uint)ELevel::Loading, _wstrLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_UI(const wstring& _wstrLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Loading::Open_Level()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CLevel* pLevel = nullptr;

	switch (m_eNextLevel)
	{
	case ELevel::AnimationTool:
		pLevel = CLevel_AnimationTool::Create(m_pDevice, m_pDeviceContext);
		break;
	}

	if (FAILED(pGameInstance->Open_Level((_uint)m_eNextLevel, pLevel)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, const ELevel _eNextLevel)
{
	CLevel_Loading* pInstance = new CLevel_Loading(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_eNextLevel)))
	{
		MSGBOX("Failed to Creating CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	Safe_Release(m_pLoader);

	__super::Free();
}
