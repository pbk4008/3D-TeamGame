#include "pch.h"
#include "UI_Death.h"
#include "SingleImage.h"
#include "UI_Fill_Space.h"
#include "UI_Blank_Space.h"

CUI_Death::CUI_Death(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}	

CUI_Death::CUI_Death(const CUI_Death& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Death::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Death::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f, 0.f, 0.4f, 1.f });
	m_pTransform->Scaling(_fvector{ 1280.f, 720.f, 1.f, 0.f });

	setActive(true);

	return S_OK;
}

_int CUI_Death::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;

	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_Death::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;

	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);

	if (this->getActive())
	{
		m_pBlankSpaceKey->setActive(true);
		m_pFillSpaceKey->setActive(true);
	}

	return _int();
}

HRESULT CUI_Death::Render()
{
	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);
	
	return S_OK;
}

HRESULT CUI_Death::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_Guide_Death";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.fColor = { 1.f, 1.f, 1.f, 0.0f };
	ModalSprite.bFadeOption = true;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	//Death
	list<CGameObject*>* listFillKeys = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI_FillSpace");
	if (listFillKeys)
	{
		m_pFillSpaceKey = (CUI_Fill_Space*)listFillKeys->front();
	}
	list<CGameObject*>* listBlankKeys = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_UI_BlankSpace");
	if (listBlankKeys)
	{
		m_pBlankSpaceKey = (CUI_Blank_Space*)listBlankKeys->front();
	}

	return S_OK;
}

void CUI_Death::HideSpaceKey(void)
{  
	this->setActive(false);

	m_pBlankSpaceKey->setActive(false);
	m_pFillSpaceKey->setActive(false);

	m_pFillSpaceKey->ResetVal(); /* 리스폰 UI용 변수 초기화 */
	m_pSigleImageCom->ResetAlpha();
}

CUI_Death* CUI_Death::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Death* pInstance = new CUI_Death(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Death Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Death::Clone(const _uint iSceneID, void* pArg)
{
	CUI_Death* pInstance = new CUI_Death(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_Death Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Death::Free()
{
	Safe_Release(m_pSigleImageCom);

	__super::Free();
}
