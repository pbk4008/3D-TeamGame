#include "pch.h"
#include "UI_EquipedText.h"
#include "SingleImage.h"

CUI_EquipedText::CUI_EquipedText(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_EquipedText::CUI_EquipedText(const CUI_EquipedText& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_EquipedText::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_EquipedText::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fPos.x , desc.fPos.y - 28.f, 0.2f, 1.f });
	m_pTransform->Scaling(_vector{ desc.fScale.x, desc.fScale.y - 60.f, 1.f, 1.f });

	if (FAILED(Ready_Component()))
		return E_FAIL;

	setActive(false);

	return S_OK;
}

_int CUI_EquipedText::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_EquipedText::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	/*if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);*/

	return _int();
}

HRESULT CUI_EquipedText::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CUI_EquipedText::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_LootNotification_BG";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

void CUI_EquipedText::SetIcon(const std::wstring& _szFileName)
{
	m_pSigleImageCom->SetTexture(_szFileName);
}

CUI_EquipedText* CUI_EquipedText::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_EquipedText* pInstance = new CUI_EquipedText(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_EquipedText Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_EquipedText::Clone(const _uint iSceneID, void* pArg)
{
	CUI_EquipedText* pInstance = new CUI_EquipedText(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_EquipedText Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_EquipedText::Free()
{
	Safe_Release(m_pSigleImageCom);
	__super::Free();
}