#include "pch.h"
#include "UI_SlotGrade.h"

#include "SingleImage.h"
#include "UIHelper.h"
#include "UI_ItemSlot.h"

CUI_SlotGrade::CUI_SlotGrade(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_SlotGrade::CUI_SlotGrade(const CUI_SlotGrade& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SlotGrade::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SlotGrade::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(ItemSlotDesc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fPos.x, desc.fPos.y, 0.5f, 1.f });
	m_pLocalTransform->Scaling(_vector{ desc.fScale.x - 3.f, desc.fScale.y - 3.f, 1.f, 1.f });

	m_pOwner = desc.pOwner;
	assert("Owner is nullptr!" && m_pOwner);
	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;


	return S_OK;
}

_int CUI_SlotGrade::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_SlotGrade::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	Attach_Owner();

	return _int();
}

HRESULT CUI_SlotGrade::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CUI_SlotGrade::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.renderType = CSingleImage::RenderType::Nonalpha;
	ModalSprite.textureName = L"T_Weapon_Sash_Common";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

void CUI_SlotGrade::SetGrade(std::wstring _szTextureName)
{
	m_pSigleImageCom->SetTexture(_szTextureName);
}

_int CUI_SlotGrade::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CUI_ItemSlot*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

CUI_SlotGrade* CUI_SlotGrade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_SlotGrade* pInstance = new CUI_SlotGrade(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_SlotGrade Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SlotGrade::Clone(const _uint iSceneID, void* pArg)
{
	CUI_SlotGrade* pInstance = new CUI_SlotGrade(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_SlotGrade Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}



void CUI_SlotGrade::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}
