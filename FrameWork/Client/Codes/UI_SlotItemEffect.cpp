#include "pch.h"
#include "UI_SlotItemEffect.h"
#include "SingleImage.h"

CUI_SlotItemEffect::CUI_SlotItemEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_SlotItemEffect::CUI_SlotItemEffect(const CUI_SlotItemEffect& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SlotItemEffect::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SlotItemEffect::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(ItemSlotDesc*)pArg);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fPos.x, desc.fPos.y, 0.3f, 1.f });
	m_pTransform->Scaling(_vector{ desc.fScale.x + 5.f, desc.fScale.y + 5.f, 1.f, 1.f });

	if (FAILED(Ready_Component()))
		return E_FAIL;

	setActive(false);

	return S_OK;
}

_int CUI_SlotItemEffect::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_SlotItemEffect::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	/*if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);*/

	return _int();
}

HRESULT CUI_SlotItemEffect::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CUI_SlotItemEffect::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_Item_Glow_Grey";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

void CUI_SlotItemEffect::SetIcon(const std::wstring& _szFileName)
{
	m_pSigleImageCom->SetTexture(_szFileName);
}

CUI_SlotItemEffect* CUI_SlotItemEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_SlotItemEffect* pInstance = new CUI_SlotItemEffect(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_SlotItemEffect Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SlotItemEffect::Clone(const _uint iSceneID, void* pArg)
{
	CUI_SlotItemEffect* pInstance = new CUI_SlotItemEffect(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_SlotItemEffect Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_SlotItemEffect::Free()
{
	Safe_Release(m_pSigleImageCom);
	__super::Free();
}
