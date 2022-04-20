#include "pch.h"
#include "UI_SelectEffect.h"
#include "SingleImage.h"
#include "UI_ItemSlot.h"

CUI_SelectEffect::CUI_SelectEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_SelectEffect::CUI_SelectEffect(const CUI_SelectEffect& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SelectEffect::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SelectEffect::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fPos.x , desc.fPos.y, 0.1f, 1.f });
	m_pLocalTransform->Scaling(_vector{ desc.fScale.x, desc.fScale.y, 1.f, 1.f });
	m_pOwner = desc.pOwner;
	assert("Owner is nullptr!" && m_pOwner);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	setActive(false);

	return S_OK;
}

_int CUI_SelectEffect::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_SelectEffect::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	Attach_Owner();



	//if (nullptr != m_pRenderer)
	//	m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	return _int();
}

HRESULT CUI_SelectEffect::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CUI_SelectEffect::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_Item_Bg_Blue_Primal";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

_int CUI_SelectEffect::Attach_Owner(void)
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CUI_ItemSlot*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

void CUI_SelectEffect::SetIcon(const std::wstring& _szFileName)
{
	m_pSigleImageCom->SetTexture(_szFileName);
}

CUI_SelectEffect* CUI_SelectEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_SelectEffect* pInstance = new CUI_SelectEffect(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_SelectEffect Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SelectEffect::Clone(const _uint iSceneID, void* pArg)
{
	CUI_SelectEffect* pInstance = new CUI_SelectEffect(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_SelectEffect Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_SelectEffect::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}