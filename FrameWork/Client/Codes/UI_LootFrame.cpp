#include "pch.h"
#include "UI_LootFrame.h"
#include "SingleImage.h"
#include "UI_ItemSlot.h"
#include "Loot_Equipment.h"

CUI_LootFrame::CUI_LootFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CHud(pDevice, pDeviceContext)
{
}

CUI_LootFrame::CUI_LootFrame(const CUI_LootFrame& rhs)
	: CHud(rhs)
{
}

HRESULT CUI_LootFrame::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_LootFrame::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(CHud::Desc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fInitPos.x + 89.f, desc.fInitPos.y + 2.f, 0.5f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 70.f, 70.f, 1.f, 0.f });

	m_pOwner = desc.pOwner;
	assert("Owner is nullptr!" && m_pOwner);

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_LootFrame::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_LootFrame::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;
	Attach_Owner();

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	return _int();
}

HRESULT CUI_LootFrame::Render()
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

HRESULT CUI_LootFrame::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_LootItemFrame";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	//ModalSprite.renderType = CSingleImage::Nonalpha;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

_int CUI_LootFrame::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CLoot_Equipment*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

CUI_LootFrame* CUI_LootFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_LootFrame* pInstance = new CUI_LootFrame(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_LootFrame Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_LootFrame::Clone(const _uint iSceneID, void* pArg)
{
	CUI_LootFrame* pInstance = new CUI_LootFrame(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_LootFrame Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_LootFrame::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}
