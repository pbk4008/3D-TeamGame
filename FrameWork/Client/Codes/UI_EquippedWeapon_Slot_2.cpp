#include "pch.h"
#include "UI_EquippedWeapon_Slot_2.h"
#include "SingleImage.h"

CUI_EquippedWeapon_Slot_2::CUI_EquippedWeapon_Slot_2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_EquippedWeapon_Slot_2::CUI_EquippedWeapon_Slot_2(const CUI_EquippedWeapon_Slot_2& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_EquippedWeapon_Slot_2::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_EquippedWeapon_Slot_2::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 815.f, -430.f, 0.2f, 1.f });
	m_pTransform->Scaling(_fvector{ 200.f, 40.f, 0.f, 0.0f });

	setActive(false);
	return S_OK;
}

_int CUI_EquippedWeapon_Slot_2::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_EquippedWeapon_Slot_2::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);

	UpdateSlot(TimeDelta);
	



	return _int();
}

HRESULT CUI_EquippedWeapon_Slot_2::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CUI_EquippedWeapon_Slot_2::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_EquippedWeapon_Border_Longsword";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

void CUI_EquippedWeapon_Slot_2::SetImage(std::wstring szTexName)
{
	m_pSigleImageCom->SetTexture(szTexName);
}

void CUI_EquippedWeapon_Slot_2::UpdateSlot(_double TimeDelta)
{

}

CUI_EquippedWeapon_Slot_2* CUI_EquippedWeapon_Slot_2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_EquippedWeapon_Slot_2* pInstance = new CUI_EquippedWeapon_Slot_2(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_EquippedWeapon_Slot_2 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_EquippedWeapon_Slot_2::Clone(const _uint iSceneID, void* pArg)
{
	CUI_EquippedWeapon_Slot_2* pInstance = new CUI_EquippedWeapon_Slot_2(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_EquippedWeapon_Slot_2 Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_EquippedWeapon_Slot_2::Free()
{
	__super::Free();

	Safe_Release(m_pSigleImageCom);
}
