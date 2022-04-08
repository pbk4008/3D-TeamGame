#include "pch.h"
#include "UI_Background.h"
#include "SingleImage.h"

CUI_Background::CUI_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_Background::CUI_Background(const CUI_Background& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Background::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Background::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransform->Scaling(_vector{ 3840.f, 1327.f });
	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ HALF_WINCX, HALF_WINCY });
	
	setActive(false);

	return S_OK;
}

_int CUI_Background::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_Background::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	m_pSigleImageCom->LateTick(TimeDelta);

	return _int();
}

HRESULT CUI_Background::Render()
{
	if(FAILED(m_pSigleImageCom->Render(m_pTransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Background::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_ModalPrompt_BG";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
		
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

CUI_Background* CUI_Background::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Background* pInstance = new CUI_Background(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Background Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Background::Clone(const _uint iSceneID, void* pArg)
{
	CUI_Background* pInstance = new CUI_Background(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_Background Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Background::Free()
{
	Safe_Release(m_pSigleImageCom);

	__super::Free();
}
