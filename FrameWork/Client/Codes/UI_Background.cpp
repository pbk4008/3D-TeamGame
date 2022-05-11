#include "pch.h"
#include "UI_Background.h"
#include "SingleImage.h"

CUI_Background::CUI_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_Background::CUI_Background(const CUI_Background& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Background::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Background::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, m_fInitPos, 0.9f, 1.f});
	m_pTransform->Scaling(_vector{ 2220.f, 1180.f, 1.f, 1.f });

	if (FAILED(Ready_Component()))
		return E_FAIL;

	setActive(false);

	return S_OK;
}

_int CUI_Background::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_Background::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (m_fInitPos > m_fEndPos)
	{
		m_fInitPos -= (_float)TimeDelta * 100.f;
		if (m_fInitPos < m_fEndPos)
		{
			m_fInitPos = m_fEndPos;
		}
		m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, m_fInitPos, 0.9f, 1.f });
	}

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	return _int();
}

HRESULT CUI_Background::Render()
{
	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);
	
	return S_OK;
}

void CUI_Background::setActive(_bool bActive)
{
	this->m_bActive = bActive;

	if (false == bActive)
		m_fInitPos = -20.f;
}

HRESULT CUI_Background::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_ModalPrompt_BG";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.fColor = { 1.f, 1.f, 1.f, 0.0f };

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

CUI_Background* CUI_Background::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Background* pInstance = new CUI_Background(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Background Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Background::Clone(const _uint iSceneID, void* pArg)
{
	CUI_Background* pInstance = new CUI_Background(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_Background Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Background::Free()
{
	Safe_Release(m_pSigleImageCom);

	__super::Free();
}
