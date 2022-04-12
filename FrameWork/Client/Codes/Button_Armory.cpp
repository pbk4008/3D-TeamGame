#include "pch.h"
#include "Button_Armory.h"
#include "SingleImage.h"
#include "UIHelper.h"

CButton_Armory::CButton_Armory(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CButton_Armory::CButton_Armory(const CButton_Armory& rhs)
	: CUI(rhs)
{
}

HRESULT CButton_Armory::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CButton_Armory::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fPos.x, desc.fPos.y, 0.4f, 1.f });
	m_pTransform->Scaling(_vector{ desc.fScale.x, desc.fScale.y, 1.f, 1.f });

	if (FAILED(Ready_Component()))
		return E_FAIL;

	setActive(false);

	return S_OK;
}

_int CButton_Armory::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CButton_Armory::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	return _int();
}

HRESULT CButton_Armory::Render()
{
	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CButton_Armory::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_Button_Armory";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	/* for. UI Helper Com */
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_UIHelper", L"UIHelper", (CComponent**)&m_pUIHelperCom)))
		return E_FAIL;

	m_pUIHelperCom->UpdateBoundary(m_pTransform);

	return S_OK;
}

_bool CButton_Armory::ButtonClicked(void)
{
	return m_pUIHelperCom->MouseClickArmoryBtn();
}

CButton_Armory* CButton_Armory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CButton_Armory* pInstance = new CButton_Armory(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CButton_Armory Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton_Armory::Clone(const _uint iSceneID, void* pArg)
{
	CButton_Armory* pInstance = new CButton_Armory(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CButton_Armory Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CButton_Armory::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pUIHelperCom);

	__super::Free();
}
