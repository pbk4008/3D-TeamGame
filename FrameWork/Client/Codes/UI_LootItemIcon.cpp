#include "pch.h"
#include "UI_LootItemIcon.h"
#include "SingleImage.h"
#include "UI_ItemSlot.h"
#include "Loot_Equipment.h"

CUI_LootItemIcon::CUI_LootItemIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CHud(pDevice, pDeviceContext)
{
}

CUI_LootItemIcon::CUI_LootItemIcon(const CUI_LootItemIcon& rhs)
	: CHud(rhs)
{
}

HRESULT CUI_LootItemIcon::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_LootItemIcon::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(CHud::Desc*)pArg);
	
	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fInitPos.x + 90.f, desc.fInitPos.y + 2.5f, 0.3f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 65.f, 65.f, 1.f, 0.f });

	m_pOwner = desc.pOwner;
	assert("Owner is nullptr!" && m_pOwner);

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_LootItemIcon::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_LootItemIcon::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;
	Attach_Owner();

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	return _int();
}

HRESULT CUI_LootItemIcon::Render()
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

HRESULT CUI_LootItemIcon::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_Weapon_Sword_1H_Player_Eclipse_Gold";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

_int CUI_LootItemIcon::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CLoot_Equipment*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

void CUI_LootItemIcon::SetIcon(std::wstring _szTextureName)
{
	m_pSigleImageCom->SetTexture(_szTextureName);
}

CUI_LootItemIcon* CUI_LootItemIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_LootItemIcon* pInstance = new CUI_LootItemIcon(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_LootItemIcon Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_LootItemIcon::Clone(const _uint iSceneID, void* pArg)
{
	CUI_LootItemIcon* pInstance = new CUI_LootItemIcon(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_LootItemIcon Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_LootItemIcon::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);
}
