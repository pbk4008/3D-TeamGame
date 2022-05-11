#include "pch.h"
#include "Subtitles.h"
#include "SingleImage.h"

CSubtitles::CSubtitles(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CSubtitles::CSubtitles(const CSubtitles& rhs)
	: CUI(rhs)
{
}

HRESULT CSubtitles::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSubtitles::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;


	if (nullptr != pArg)
		desc = (*(Desc*)pArg);

	/* for. check player Pos */
	if (!desc.bUsingCinema)
	{
		m_pTriggerTrans = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
		m_pTriggerTrans->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&desc.fPos));
	}

	/* for. Texture Render*/
	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f, -415.f, 0.1f, 1.f });
	m_pTransform->Scaling(_fvector{ 720.f, 40.5f, 0.f, 0.f });

	setActive(false);

	return S_OK;
}

_int CSubtitles::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	if (!desc.bUsingCinema)
	{
		m_pPlayer = *g_pGameInstance->getObjectList(g_pGameInstance->getCurrentLevel(), L"Layer_Silvermane")->begin();
		_float dist = MathUtils::Length(m_pPlayer, m_pTriggerTrans);

		if (dist <= m_interactDist)
			this->setActive(true);
	}
	//else
	//	this->setActive(false);

	return _int();
}

_int CSubtitles::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);



	return _int();
}

HRESULT CSubtitles::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CSubtitles::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_Voice_IsYourArmor_Ravenna";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.bFadeOption = true;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

void CSubtitles::SetImage(std::wstring szTexName)
{
	m_pSigleImageCom->SetTexture(szTexName);
}

void CSubtitles::UpScaleing(void)
{
	m_pTransform->Scaling(_fvector{ 620.f, 36.f, 0.f, 0.f });
}

void CSubtitles::DownScaleing(void)
{
	m_pTransform->Scaling(_fvector{ 520.f, 33.f, 0.f, 0.f });
}

CSubtitles* CSubtitles::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CSubtitles* pInstance = new CSubtitles(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CSubtitles Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSubtitles::Clone(const _uint iSceneID, void* pArg)
{
	CSubtitles* pInstance = new CSubtitles(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CSubtitles Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSubtitles::Free()
{
	__super::Free();

	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pTriggerTrans);
}
