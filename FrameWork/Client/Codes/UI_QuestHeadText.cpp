#include "pch.h"
#include "Quest.h"
#include "UI_QuestHeadText.h"
#include "SingleImage.h"

CUI_QuestHeadText::CUI_QuestHeadText(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_QuestHeadText::CUI_QuestHeadText(const CUI_QuestHeadText& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_QuestHeadText::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_QuestHeadText::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ -870.f, 308.f, 0.02f, 1.f });
	m_pTransform->Scaling(_vector{ 110.f , 48.f, 1.f, 0.f });

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_QuestHeadText::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_QuestHeadText::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);
	return _int();
}

HRESULT CUI_QuestHeadText::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;
	if (!g_pInvenUIManager->IsOpenModal() &&
		!g_pGuideManager->IsOpenDeathUI())
	{
		m_pSigleImageCom->Render(m_pTransform);
	}
	return S_OK;
}

HRESULT CUI_QuestHeadText::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = g_arrQuestHeaderTex[(_int)desc.EHeadText];
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

void CUI_QuestHeadText::SetIcon(std::wstring _szTextureName)
{
	m_pSigleImageCom->SetTexture(_szTextureName);
}

CUI_QuestHeadText* CUI_QuestHeadText::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_QuestHeadText* pInstance = new CUI_QuestHeadText(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_QuestHeadText Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_QuestHeadText::Clone(const _uint iSceneID, void* pArg)
{
	CUI_QuestHeadText* pInstance = new CUI_QuestHeadText(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_QuestHeadText Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_QuestHeadText::Free()
{
	Safe_Release(m_pSigleImageCom);
	__super::Free();
}
