#include "pch.h"
#include "UI_Equipment.h"
#include "SingleImage.h"
#include "InventoryData.h"

CUI_Equipment::CUI_Equipment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_Equipment::CUI_Equipment(const CUI_Equipment& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Equipment::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Equipment::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransform->Scaling(_vector{ 1280.f, 720.f });
	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ HALF_WINCX, HALF_WINCY });

	m_pEquipData = g_pDataManager->GET_DATA(CInventoryData, L"InventoryData");

	setActive(false);
}

_int CUI_Equipment::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_Equipment::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	m_pSigleImageCom->LateTick(TimeDelta);
}

HRESULT CUI_Equipment::Render()
{
	if (FAILED(m_pSigleImageCom->Render(m_pTransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Equipment::Ready_Component(void)
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

void CUI_Equipment::Show(void)
{
	UpdateSlots();
	UpdateResourceCount();
}

void CUI_Equipment::Hide(void)
{
}

void CUI_Equipment::UpdateSlots(void)
{
}

void CUI_Equipment::UpdateSlot(_int _iIndex)
{
}

void CUI_Equipment::UpdateResourceCount(void)
{
}

void CUI_Equipment::ClickSlot(void)
{
}

void CUI_Equipment::MouseOnSlot(void)
{
}

_bool CUI_Equipment::GetEquipmentActive(void)
{
	return _bool();
}

void CUI_Equipment::InsertSlotVector()
{
}

void CUI_Equipment::SetResourceCount()
{
}

CUI_Equipment* CUI_Equipment::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Equipment* pInstance = new CUI_Equipment(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_Equipment Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Equipment::Clone(const _uint iSceneID, void* pArg)
{
	CUI_Equipment* pInstance = new CUI_Equipment(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_Equipment Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Equipment::Free()
{
	Safe_Release(m_pSigleImageCom);

	__super::Free();
}
