#include "pch.h"
#include "UI_LevelUP_Fill_Lead_Left.h"
#include "SingleImage.h"
#include "UI_ItemSlot.h"
#include "UI_Level_UP.h"
#include "UI_LevelUP_Fill_Right.h"

CUI_LevelUP_Fill_Lead_Left::CUI_LevelUP_Fill_Lead_Left(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_LevelUP_Fill_Lead_Left::CUI_LevelUP_Fill_Lead_Left(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_LevelUP_Fill_Lead_Left::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_LevelUP_Fill_Lead_Left::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ -40.f , -404.f, 0.2f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 30.f, 8.f, 1.f, 0.f });

	m_pOwner = desc.pOwner;
	assert(m_pOwner);

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_LevelUP_Fill_Lead_Left::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_LevelUP_Fill_Lead_Left::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;


	_float Gap = 0.f;
	if (nullptr != m_pFill)
	{
		m_fRatio = m_pFill->Get_Ratio();
		Gap = m_fRatio * (XMVectorGetX(m_pFill->Get_Transform()->Get_State(CTransform::STATE_RIGHT)) - 20.f);
	}

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ -40.f - Gap , -404.f, 0.05f, 1.f });

	Attach_Owner();


	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_TOP, this);

	return _int();
}

HRESULT CUI_LevelUP_Fill_Lead_Left::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);
			
	return S_OK;
}

HRESULT CUI_LevelUP_Fill_Lead_Left::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_LevelUp_Fill_Lead_Left";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.renderType = CSingleImage::Alpha;
	ModalSprite.bFadeOption = true;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

_int CUI_LevelUP_Fill_Lead_Left::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CUI_LevelUP_Fill_Lead_Left*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}
  
void CUI_LevelUP_Fill_Lead_Left::SetBg(const std::wstring& _szFileName)
{
	m_pSigleImageCom->SetTexture(_szFileName);
}

void CUI_LevelUP_Fill_Lead_Left::SetFadeOut(void)
{
	m_pSigleImageCom->SetFadeOut();
}

void CUI_LevelUP_Fill_Lead_Left::FadeIn(void)
{
	m_pSigleImageCom->SetFadeOutFalse();
}

CUI_LevelUP_Fill_Lead_Left* CUI_LevelUP_Fill_Lead_Left::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_LevelUP_Fill_Lead_Left* pInstance = new CUI_LevelUP_Fill_Lead_Left(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_LevelUP_Fill_Lead_Left Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_LevelUP_Fill_Lead_Left::Clone(const _uint iSceneID, void* pArg)
{
	CUI_LevelUP_Fill_Lead_Left* pInstance = new CUI_LevelUP_Fill_Lead_Left(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_LevelUP_Fill_Lead_Left Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_LevelUP_Fill_Lead_Left::Free()
{
	__super::Free();

	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);
}
