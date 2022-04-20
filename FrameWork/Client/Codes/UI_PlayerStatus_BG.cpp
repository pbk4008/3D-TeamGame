#include "pch.h"
#include "UI_PlayerStatus_BG.h"
#include "UI_PlayerStatusWindow.h"
#include "SingleImage.h"

CUI_PlayerStatus_BG::CUI_PlayerStatus_BG(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_PlayerStatus_BG::CUI_PlayerStatus_BG(const CUI_PlayerStatus_BG& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PlayerStatus_BG::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_PlayerStatus_BG::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ 440.f, 0.f, 0.7f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 360.f, 580.f, 1.f, 1.f });

	m_pOwner = desc.pOwner;

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_PlayerStatus_BG::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_PlayerStatus_BG::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	Attach_Owner();

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ 430.f, 0.f, 0.6f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 360.f, 600.f, 1.f, 1.f });

	return _int();
}

HRESULT CUI_PlayerStatus_BG::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	m_pSigleImageCom->Render(m_pTransform);
			
	return S_OK;
}

HRESULT CUI_PlayerStatus_BG::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_Player_Status_Bg";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

_int CUI_PlayerStatus_BG::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CUI_PlayerStatusWindow*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

CUI_PlayerStatus_BG* CUI_PlayerStatus_BG::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_PlayerStatus_BG* pInstance = new CUI_PlayerStatus_BG(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_PlayerStatus_BG Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerStatus_BG::Clone(const _uint iSceneID, void* pArg)
{
	CUI_PlayerStatus_BG* pInstance = new CUI_PlayerStatus_BG(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_PlayerStatus_BG Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_PlayerStatus_BG::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}
