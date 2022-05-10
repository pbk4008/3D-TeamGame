#include "pch.h"
#include "UI_Guide_Texture.h"
#include "SingleImage.h"

CUI_Guide_Texture::CUI_Guide_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_Guide_Texture::CUI_Guide_Texture(const CUI_Guide_Texture& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Guide_Texture::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Guide_Texture::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if(nullptr != pArg)
		desc = (*(Desc*)pArg);

	/* for. check player Pos */
	m_pTriggerTrans = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pTriggerTrans->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&desc.fPos));

	/* for. Texture Render*/
	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 20.f, 0.f, 0.1f, 1.f });
	m_pTransform->Scaling(_fvector{ 800.f, 450.f, 1.f, 0.f });

	setActive(false);

	return S_OK;
}

_int CUI_Guide_Texture::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;

	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	m_pPlayer = *g_pGameInstance->getObjectList(g_pGameInstance->getCurrentLevel(), L"Layer_Silvermane")->begin();
	_float dist = MathUtils::Length(m_pPlayer, m_pTriggerTrans);

	if (dist <= m_interactDist)
		this->setActive(true);
	else
		this->setActive(false);

	if (true == getActive() && false == m_bSoundPlay)
	{
		STOP_SOUND(CHANNEL::GUIDE);
		PLAY_SOUND(L"UI_GuideImg", CHANNEL::GUIDE);
		VOLUME_CHANGE(CHANNEL::GUIDE, 0.5f);
		m_bSoundPlay = true;
	}
	
	return _int();
}

_int CUI_Guide_Texture::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;

	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(desc.ERenderGroup, this);

	m_pSigleImageCom->LateTick(TimeDelta);
	m_pTransform->Scaling(_fvector{ 800.f, 500.f, 1.f, 0.f });
	return _int();
}

HRESULT CUI_Guide_Texture::Render()
{
	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);
	
	return S_OK;
}

HRESULT CUI_Guide_Texture::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_Guide_JumpNode";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.fColor = { 1.f, 1.f, 1.f, 0.0f };
	ModalSprite.bFadeOption = true;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

void CUI_Guide_Texture::SetTexture(std::wstring szTexture)
{
	m_pSigleImageCom->SetTexture(szTexture);
	//m_pSigleImageCom->SetFadeOut();
}

CUI_Guide_Texture* CUI_Guide_Texture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Guide_Texture* pInstance = new CUI_Guide_Texture(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Guide_Texture Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Guide_Texture::Clone(const _uint iSceneID, void* pArg)
{
	CUI_Guide_Texture* pInstance = new CUI_Guide_Texture(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_Guide_Texture Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Guide_Texture::Free()
{
	__super::Free();

	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pTriggerTrans);
}
