#include "pch.h"
#include "UI_PlayerStatus_DPS_2.h"
#include "UI_PlayerStatusWindow.h"
#include "SingleImage.h"

CUI_PlayerStatus_DPS_2::CUI_PlayerStatus_DPS_2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_PlayerStatus_DPS_2::CUI_PlayerStatus_DPS_2(const CUI_PlayerStatus_DPS_2& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PlayerStatus_DPS_2::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_PlayerStatus_DPS_2::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ 540.f, 45.f, 0.5f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 30.f, 30.f, 1.f, 1.f });

	m_pOwner = desc.pOwner;

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_PlayerStatus_DPS_2::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_PlayerStatus_DPS_2::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	Attach_Owner();

	return _int();
}

HRESULT CUI_PlayerStatus_DPS_2::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	m_pSigleImageCom->Render(m_pTransform);
			
	return S_OK;
}

HRESULT CUI_PlayerStatus_DPS_2::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_1H_DPS_100";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

_int CUI_PlayerStatus_DPS_2::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CUI_PlayerStatusWindow*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

void CUI_PlayerStatus_DPS_2::SetIcon(std::wstring szIconName)
{
	m_pSigleImageCom->SetTexture(szIconName);
}

CUI_PlayerStatus_DPS_2* CUI_PlayerStatus_DPS_2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_PlayerStatus_DPS_2* pInstance = new CUI_PlayerStatus_DPS_2(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_PlayerStatus_DPS_2 Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerStatus_DPS_2::Clone(const _uint iSceneID, void* pArg)
{
	CUI_PlayerStatus_DPS_2* pInstance = new CUI_PlayerStatus_DPS_2(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_PlayerStatus_DPS_2 Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_PlayerStatus_DPS_2::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}
