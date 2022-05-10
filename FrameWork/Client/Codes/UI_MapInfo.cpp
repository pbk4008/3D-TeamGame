#include "pch.h"
#include "UI_MapInfo.h"
#include "SingleImage.h"

CMapInfo::CMapInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CMapInfo::CMapInfo(const CMapInfo& rhs)
	: CUI(rhs)
{
}

HRESULT CMapInfo::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapInfo::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	/* for. Texture Render*/
	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f, 290.f, 0.1f, 1.f });
	m_pTransform->Scaling(_fvector{ 420.f, 105.f, 0.f, 0.f });

	setActive(false);

	return S_OK;
}

_int CMapInfo::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CMapInfo::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f, 400.f, 0.1f, 1.f });


	return _int();
}

HRESULT CMapInfo::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CMapInfo::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_Stage_1";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.bFadeOption = true;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

void CMapInfo::SetImage(std::wstring szTexName)
{
	m_pSigleImageCom->SetTexture(szTexName);
}

void CMapInfo::FadeOut(void)
{
	m_pSigleImageCom->SetFadeOut();
}

CMapInfo* CMapInfo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMapInfo* pInstance = new CMapInfo(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMapInfo Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMapInfo::Clone(const _uint iSceneID, void* pArg)
{
	CMapInfo* pInstance = new CMapInfo(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CMapInfo Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMapInfo::Free()
{
	__super::Free();

	Safe_Release(m_pSigleImageCom);
}
