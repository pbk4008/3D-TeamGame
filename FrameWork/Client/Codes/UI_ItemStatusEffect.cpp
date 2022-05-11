#include "pch.h"
#include "UI_ItemStatusEffect.h"
#include "SingleImage.h"
#include "UI_ItemStatusWindow.h"

UI_ItemStatusEffect::UI_ItemStatusEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

UI_ItemStatusEffect::UI_ItemStatusEffect(const UI_ItemStatusEffect& rhs)
	: CUI(rhs)
{
}

HRESULT UI_ItemStatusEffect::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT UI_ItemStatusEffect::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ 600.f, 180.f, 0.1f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 340.f, 340.f, 1.f, 0.f });

	if (FAILED(Ready_Component()))
		return E_FAIL;


	setActive(true);

	return S_OK;
}

_int UI_ItemStatusEffect::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int UI_ItemStatusEffect::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	m_pLocalTransform->Rotation_Axis(_vector{ 0.f, 0.f, 1.f, 0.f }, TimeDelta);

	Attach_Owner();



	return _int();
}

HRESULT UI_ItemStatusEffect::Render()
{
	if (FAILED(CUI::Render()))
		return -1;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

void UI_ItemStatusEffect::setActive(_bool bActive)
{
	this->m_bActive = bActive;
}

HRESULT UI_ItemStatusEffect::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_GearPopover_Glow_Common";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	CTransform::tagTransformDesc desc;
	desc.fRotationPerSec = 1.0f;

	m_pLocalTransform->Set_TransformDesc(desc);

	return S_OK;
}

CSingleImage* UI_ItemStatusEffect::GetSingleImage(void)
{
	return m_pSigleImageCom;
}

_int UI_ItemStatusEffect::Attach_Owner(void)
{
	if (nullptr != desc.pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CUI_ItemStatusWindow*>(desc.pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}


	return _int();
}

void UI_ItemStatusEffect::SetTexture(std::wstring name)
{
	m_pSigleImageCom->SetTexture(name);
}

UI_ItemStatusEffect* UI_ItemStatusEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UI_ItemStatusEffect* pInstance = new UI_ItemStatusEffect(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("UI_ItemStatusEffect Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* UI_ItemStatusEffect::Clone(const _uint iSceneID, void* pArg)
{
	UI_ItemStatusEffect* pInstance = new UI_ItemStatusEffect(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("UI_ItemStatusEffect Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UI_ItemStatusEffect::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}
