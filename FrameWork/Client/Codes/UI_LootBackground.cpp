#include "pch.h"
#include "UI_LootBackground.h"
#include "SingleImage.h"
#include "UI_ItemSlot.h"
#include "Loot_Equipment.h"

CUI_LootBackground::CUI_LootBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CHud(pDevice, pDeviceContext)
{
}

CUI_LootBackground::CUI_LootBackground(const CUI_LootBackground& rhs)
	: CHud(rhs)
{
}

HRESULT CUI_LootBackground::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_LootBackground::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(CHud::Desc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fInitPos.x, desc.fInitPos.y, 0.6f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 250.f, 68.f, 1.f, 0.f });

	m_pOwner = desc.pOwner;
	assert("Owner is nullptr!" && m_pOwner);

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_LootBackground::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_LootBackground::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	Attach_Owner();

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	return _int();
}

HRESULT CUI_LootBackground::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (!g_pInvenUIManager->IsOpenModal())
	{
		if (this->getActive())
			m_pSigleImageCom->Render(m_pTransform);
	}

	return S_OK;
}

HRESULT CUI_LootBackground::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_LootNotification_BG";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.renderType = CSingleImage::Alpha;
	ModalSprite.bFadeOption = true;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

_int CUI_LootBackground::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CLoot_Equipment*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}
  
void CUI_LootBackground::SetBg(const std::wstring& _szFileName)
{
	m_pSigleImageCom->SetTexture(_szFileName);
}

void CUI_LootBackground::SetFadeOut(void)
{
	m_pSigleImageCom->SetFadeOut();
}

CUI_LootBackground* CUI_LootBackground::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_LootBackground* pInstance = new CUI_LootBackground(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_LootBackground Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_LootBackground::Clone(const _uint iSceneID, void* pArg)
{
	CUI_LootBackground* pInstance = new CUI_LootBackground(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_LootBackground Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_LootBackground::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}
