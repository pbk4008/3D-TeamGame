#include "pch.h"
#include "UI_Guide_Background.h"
#include "SingleImage.h"

CUI_Guide_Background::CUI_Guide_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_Guide_Background::CUI_Guide_Background(const CUI_Guide_Background& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Guide_Background::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Guide_Background::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	setActive(false);

	return S_OK;
}

_int CUI_Guide_Background::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_Guide_Background::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f, 0.f, 0.2f, 1.f });
	m_pTransform->Scaling(_fvector{ 1280.f, 720.f, 1.f, 0.f });

	return _int();
}

HRESULT CUI_Guide_Background::Render()
{
	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);
	
	return S_OK;
}

HRESULT CUI_Guide_Background::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_Guide_BackGround";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.fColor = { 1.f, 1.f, 1.f, 0.0f };
	ModalSprite.bFadeOption = true;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

CUI_Guide_Background* CUI_Guide_Background::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Guide_Background* pInstance = new CUI_Guide_Background(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Guide_Background Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Guide_Background::Clone(const _uint iSceneID, void* pArg)
{
	CUI_Guide_Background* pInstance = new CUI_Guide_Background(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_Guide_Background Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Guide_Background::Free()
{
	Safe_Release(m_pSigleImageCom);

	__super::Free();
}
