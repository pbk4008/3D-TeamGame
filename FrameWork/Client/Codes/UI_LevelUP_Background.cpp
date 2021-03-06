#include "pch.h"
#include "UI_LevelUP_Background.h"
#include "SingleImage.h"
#include "UI_Level_UP.h"

CUI_LevelUP_Background::CUI_LevelUP_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_LevelUP_Background::CUI_LevelUP_Background(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_LevelUP_Background::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_LevelUP_Background::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ 0.f , -400.f, 0.1f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 47.f, 47.f, 1.f, 0.f });

	m_pOwner = desc.pOwner;
	assert(m_pOwner);

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_LevelUP_Background::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_LevelUP_Background::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (true == m_bShow)
		Show(TimeDelta);

	Attach_Owner();

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	return _int();
}

HRESULT CUI_LevelUP_Background::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);
			
	return S_OK;
}

HRESULT CUI_LevelUP_Background::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_Level_BG_1";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	//ModalSprite.renderType = CSingleImage::Alpha;
	ModalSprite.bFadeOption = true;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	m_pSigleImageCom->SetFadeTime(3.f);

	return S_OK;
}

_int CUI_LevelUP_Background::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CUI_LevelUP_Background*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}
  
void CUI_LevelUP_Background::Show(_double dTimeDelta)
{
	if (m_fInitScale.x > m_fEndScale.x)
	{
		m_fInitScale.y = m_fInitScale.x -= (_float)dTimeDelta * 300.f;

		if (m_fInitScale.x <= m_fEndScale.x)
		{
			m_fInitScale.x = m_fEndScale.x;
			m_fInitScale.y = m_fEndScale.y;
			m_bShow = false;
		}
		m_pLocalTransform->Scaling(_vector{ m_fInitScale.x , m_fInitScale.y, 1.f, 0.f });
	}
}

void CUI_LevelUP_Background::SetBg(_int PlayerLevel)
{
	/* Level Up???? ???????? ??*/
	m_pSigleImageCom->SetTexture(m_arrLevelBgTex[PlayerLevel]);
	m_bShow = true;
}

void CUI_LevelUP_Background::SetFadeOut(void)
{
	/* ???? ???????? ???? ???? ???? ???? */
	m_pSigleImageCom->SetFadeOut();
	m_fInitScale = { 80.f, 80.f };

	m_pLocalTransform->Scaling(_vector{ 47.f , 47.f, 1.f, 0.f });
}

void CUI_LevelUP_Background::FadeIn(void)
{
	m_pSigleImageCom->SetFadeOutFalse();
}

void CUI_LevelUP_Background::FixPos(void)
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 0.f , -270.f, 0.1f, 1.f });
}

CUI_LevelUP_Background* CUI_LevelUP_Background::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_LevelUP_Background* pInstance = new CUI_LevelUP_Background(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_LevelUP_Background Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_LevelUP_Background::Clone(const _uint iSceneID, void* pArg)
{
	CUI_LevelUP_Background* pInstance = new CUI_LevelUP_Background(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_LevelUP_Background Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_LevelUP_Background::Free()
{
	__super::Free();

	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);
}
