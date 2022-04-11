#include "pch.h"
#include "UI_Armory.h"
#include "SingleImage.h"

CUI_Armory::CUI_Armory(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_Armory::CUI_Armory(const CUI_Armory& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Armory::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Armory::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransform->Scaling(_vector{ 1280.f, 720.f });
	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ HALF_WINCX, HALF_WINCY });
	
	setActive(false);

	return S_OK;
}

_int CUI_Armory::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_Armory::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	m_pSigleImageCom->LateTick(TimeDelta);

	return _int();
}

HRESULT CUI_Armory::Render()
{
	if(FAILED(m_pSigleImageCom->Render(m_pTransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Armory::Ready_Component(void)
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

CUI_Armory* CUI_Armory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Armory* pInstance = new CUI_Armory(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Armory Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Armory::Clone(const _uint iSceneID, void* pArg)
{
	CUI_Armory* pInstance = new CUI_Armory(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_Armory Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Armory::Free()
{
	Safe_Release(m_pSigleImageCom);

	__super::Free();
}
