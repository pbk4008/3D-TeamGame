#include "pch.h"
#include "UI_LootItemNameType.h"
#include "SingleImage.h"
#include "UI_ItemSlot.h"
#include "Loot_Equipment.h"

CUI_LootItemNameType::CUI_LootItemNameType(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CHud(pDevice, pDeviceContext)
{
}

CUI_LootItemNameType::CUI_LootItemNameType(const CUI_LootItemNameType& rhs)
	: CHud(rhs)
{
}

HRESULT CUI_LootItemNameType::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_LootItemNameType::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(CHud::Desc*)pArg);
	
	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fInitPos.x + 20.f, desc.fInitPos.y - 15.f, 0.2f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 50.f , 25.f, 1.f, 0.f });

	m_pOwner = desc.pOwner;
	assert("Owner is nullptr!" && m_pOwner);

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_LootItemNameType::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_LootItemNameType::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;
	Attach_Owner();

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fInitPos.x - 10.f, desc.fInitPos.y + 15.f, 0.2f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 95.f, 27.f, 1.f, 0.f });
	//if (nullptr != m_pRenderer)
	//	m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	return _int();
}

HRESULT CUI_LootItemNameType::Render()
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

HRESULT CUI_LootItemNameType::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_WeaponName_Heedong";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

_int CUI_LootItemNameType::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CLoot_Equipment*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

void CUI_LootItemNameType::SetIcon(std::wstring _szTextureName)
{
	m_pSigleImageCom->SetTexture(_szTextureName);
}

CUI_LootItemNameType* CUI_LootItemNameType::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_LootItemNameType* pInstance = new CUI_LootItemNameType(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_LootItemNameType Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_LootItemNameType::Clone(const _uint iSceneID, void* pArg)
{
	CUI_LootItemNameType* pInstance = new CUI_LootItemNameType(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_LootItemNameType Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_LootItemNameType::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);
}
