#include "pch.h"
#include "UI_EquippedWeapon_Slot_1.h"
#include "SingleImage.h"

CUI_EquippedWeapon_Slot_1::CUI_EquippedWeapon_Slot_1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_EquippedWeapon_Slot_1::CUI_EquippedWeapon_Slot_1(const CUI_EquippedWeapon_Slot_1& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_EquippedWeapon_Slot_1::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_EquippedWeapon_Slot_1::NativeConstruct(const _uint iSceneID, void* pArg)
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

_int CUI_EquippedWeapon_Slot_1::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_EquippedWeapon_Slot_1::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);

	UpdateSlot(TimeDelta);

	if (true == m_IsRightMoveEnd)
		UpdateSlotImage(TimeDelta);

	return _int();
}

HRESULT CUI_EquippedWeapon_Slot_1::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CUI_EquippedWeapon_Slot_1::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_EquippedWeapon_Border_Longsword";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.bFadeOption = true;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	m_pSigleImageCom->SetFadeTime(5);

	return S_OK;
}

void CUI_EquippedWeapon_Slot_1::SetImage(std::wstring szTexName)
{
	m_pSigleImageCom->SetTexture(szTexName);
}

void CUI_EquippedWeapon_Slot_1::UpdateSlot(_double TimeDelta)
{
	if (true == m_IsMainSlot)
	{
		if (m_fInitPos2 < m_fEndPos2)
		{
			m_fInitPos2 += (_float)TimeDelta * 100.0f;

			if (m_fInitPos2 >= m_fEndPos2)
			{
				m_fInitPos2 = m_fEndPos;
				m_fInitPos = 720.f;
			}
			m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ m_fInitPos2, -380.f , 0.2f, 1.f });
		}
	}
	else
	{
		if (m_fInitPos < m_fEndPos)
		{
			m_fInitPos += (_float)TimeDelta * 100.0f;

			if (m_fInitPos >= m_fEndPos)
			{
				m_fInitPos = m_fEndPos;
				m_fInitPos2 = 720.f;
				m_IsRightMoveEnd = true;
			}
			m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ m_fInitPos, -380.f , 0.2f, 1.f});
		}
	}
}

void CUI_EquippedWeapon_Slot_1::UpdateSlotImage(_double TimeDelta)
{
	/* Init Pos 470 -> EndPos 450 */
	if (m_fInitPos2 > m_fEndPos2)
	{
		m_fInitPos2 -= (_float)TimeDelta * 100.f;

		if (m_fInitPos2 < m_fEndPos2)
		{
			m_fInitPos2 = m_fEndPos2;
			m_IsRightMoveEnd = false;
		}
		m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ m_fInitPos2, -200.f , 0.2f, 1.f });
	}
}

CUI_EquippedWeapon_Slot_1* CUI_EquippedWeapon_Slot_1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_EquippedWeapon_Slot_1* pInstance = new CUI_EquippedWeapon_Slot_1(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_EquippedWeapon_Slot_1 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_EquippedWeapon_Slot_1::Clone(const _uint iSceneID, void* pArg)
{
	CUI_EquippedWeapon_Slot_1* pInstance = new CUI_EquippedWeapon_Slot_1(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_EquippedWeapon_Slot_1 Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_EquippedWeapon_Slot_1::Free()
{
	__super::Free();

	Safe_Release(m_pSigleImageCom);
}
