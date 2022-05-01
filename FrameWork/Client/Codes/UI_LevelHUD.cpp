#include "pch.h"
#include "UI_LevelHUD.h"
#include "SingleImage.h"

CUI_LevelHUD::CUI_LevelHUD(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_LevelHUD::CUI_LevelHUD(const CUI_LevelHUD& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_LevelHUD::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_LevelHUD::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	/* for. Texture Render*/
	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f, 290.f, 0.1f, 1.f });
	m_pTransform->Scaling(_fvector{ 512.f, 124.f, 0.f, 0.f });

	setActive(false);
}

_int CUI_LevelHUD::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_LevelHUD::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);



	return _int();
}

HRESULT CUI_LevelHUD::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
  		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CUI_LevelHUD::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_2Level";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;
}

void CUI_LevelHUD::SetImage(std::wstring szTexName)
{
	m_pSigleImageCom->SetTexture(szTexName);
}

void CUI_LevelHUD::FadeOut(void)
{
	m_pSigleImageCom->SetFadeOut();
}

void CUI_LevelHUD::ResetAlpha(void)
{
	m_pSigleImageCom->ResetAlpha(0.f);
}

CUI_LevelHUD* CUI_LevelHUD::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_LevelHUD* pInstance = new CUI_LevelHUD(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_LevelHUD Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_LevelHUD::Clone(const _uint iSceneID, void* pArg)
{
	CUI_LevelHUD* pInstance = new CUI_LevelHUD(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_LevelHUD Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_LevelHUD::Free()
{
	__super::Free();

	Safe_Release(m_pSigleImageCom);
}
