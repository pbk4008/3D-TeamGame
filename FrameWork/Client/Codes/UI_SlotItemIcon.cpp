#include "pch.h"
#include "UI_SlotItemIcon.h"
#include "SingleImage.h"
#include "UIHelper.h"
#include "UI_ItemSlot.h"

CUI_SlotItemIcon::CUI_SlotItemIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_SlotItemIcon::CUI_SlotItemIcon(const CUI_SlotItemIcon& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SlotItemIcon::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SlotItemIcon::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(ItemSlotDesc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fPos.x, desc.fPos.y, 0.4f, 1.f });
	m_pLocalTransform->Scaling(_vector{ desc.fScale.x, desc.fScale.y, 1.f, 1.f });
	m_pOwner = desc.pOwner;
	assert("Owner is nullptr!" && m_pOwner);

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_SlotItemIcon::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_SlotItemIcon::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	Attach_Owner();

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	return _int();
}

HRESULT CUI_SlotItemIcon::Render()
{
	if (FAILED(CUI::Render()))
		return -1;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CUI_SlotItemIcon::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_Item_Bg_None";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	/* for. UI Helper Com */
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_UIHelper", L"UIHelper", (CComponent**)&m_pUIHelperCom)))
		return E_FAIL;

	m_pUIHelperCom->UpdateBoundary(m_pLocalTransform);

	return S_OK;
}

_int CUI_SlotItemIcon::Attach_Owner(void)
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CUI_ItemSlot*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

_bool CUI_SlotItemIcon::IconMouseOn(void)
{
	return m_pUIHelperCom->IsCursorRect();
}

_bool CUI_SlotItemIcon::IconClicked()
{
	return m_pUIHelperCom->MouseClickDown();
}

_bool CUI_SlotItemIcon::ItemClicked(void)
{
	return m_pUIHelperCom->MouseClickItem();
}

void CUI_SlotItemIcon::SetIcon(const std::wstring& _szFileName)
{
	m_pSigleImageCom->SetTexture(_szFileName);
	m_bHasItem = true;
}


CUI_SlotItemIcon* CUI_SlotItemIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_SlotItemIcon* pInstance = new CUI_SlotItemIcon(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_SlotItemIcon Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SlotItemIcon::Clone(const _uint iSceneID, void* pArg)
{
	CUI_SlotItemIcon* pInstance = new CUI_SlotItemIcon(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_SlotItemIcon Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_SlotItemIcon::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pUIHelperCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}