#include "pch.h"
#include "UI_PlayerStatus_Spirit.h"
#include "UI_PlayerStatusWindow.h"
#include "SingleImage.h"

CUI_PlayerStatus_Spirit::CUI_PlayerStatus_Spirit(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_PlayerStatus_Spirit::CUI_PlayerStatus_Spirit(const CUI_PlayerStatus_Spirit& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PlayerStatus_Spirit::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_PlayerStatus_Spirit::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ 600.f, 170.f, 0.5f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 53.f, 53.f, 1.f, 1.f });

	m_pOwner = desc.pOwner;

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_PlayerStatus_Spirit::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_PlayerStatus_Spirit::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	Attach_Owner();

	return _int();
}

HRESULT CUI_PlayerStatus_Spirit::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	m_pSigleImageCom->Render(m_pTransform);
			
	return S_OK;
}

HRESULT CUI_PlayerStatus_Spirit::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_INT_40";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

_int CUI_PlayerStatus_Spirit::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CUI_PlayerStatusWindow*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

void CUI_PlayerStatus_Spirit::SetIcon(std::wstring szIconName)
{
	m_pSigleImageCom->SetTexture(szIconName);
}

CUI_PlayerStatus_Spirit* CUI_PlayerStatus_Spirit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_PlayerStatus_Spirit* pInstance = new CUI_PlayerStatus_Spirit(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_PlayerStatus_Spirit Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerStatus_Spirit::Clone(const _uint iSceneID, void* pArg)
{
	CUI_PlayerStatus_Spirit* pInstance = new CUI_PlayerStatus_Spirit(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_PlayerStatus_Spirit Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_PlayerStatus_Spirit::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}
