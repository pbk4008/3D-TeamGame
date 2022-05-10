#include "pch.h"
#include "UI_EquippedWeapon.h"
#include "SingleImage.h"

CUI_EquippedWeapon::CUI_EquippedWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_EquippedWeapon::CUI_EquippedWeapon(const CUI_EquippedWeapon& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_EquippedWeapon::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_EquippedWeapon::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 740.f, -380.f, 0.2f, 1.f });
	m_pTransform->Scaling(_fvector{ 260.f, 60.f, 0.f, 0.0f });

	setActive(false);
	return S_OK;
}

_int CUI_EquippedWeapon::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_EquippedWeapon::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);

	return _int();
}

HRESULT CUI_EquippedWeapon::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CUI_EquippedWeapon::Ready_Component(void)
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

void CUI_EquippedWeapon::SetImage(std::wstring szTexName)
{
	m_pSigleImageCom->SetTexture(szTexName);
}

CUI_EquippedWeapon* CUI_EquippedWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_EquippedWeapon* pInstance = new CUI_EquippedWeapon(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_EquippedWeapon Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_EquippedWeapon::Clone(const _uint iSceneID, void* pArg)
{
	CUI_EquippedWeapon* pInstance = new CUI_EquippedWeapon(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_EquippedWeapon Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_EquippedWeapon::Free()
{
	__super::Free();

	Safe_Release(m_pSigleImageCom);
}
