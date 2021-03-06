#include "pch.h"
#include "Button_Equipment.h"
#include "SingleImage.h"
#include "UIHelper.h"
#include "UI_Indexes.h"

CButton_Equipment::CButton_Equipment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CButton_Equipment::CButton_Equipment(const CButton_Equipment& rhs)
	: CUI(rhs)
{
}

HRESULT CButton_Equipment::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CButton_Equipment::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fPos.x, desc.fPos.y, 0.4f, 1.f });
	m_pLocalTransform->Scaling(_vector{ desc.fScale.x, desc.fScale.y, 1.f, 1.f });
	m_pOwner = desc.pOwner;
	assert("Owner is nullptr!" && m_pOwner);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	setActive(false);

	return S_OK;
}

_int CButton_Equipment::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CButton_Equipment::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	Attach_Owner();

	return _int();
}

HRESULT CButton_Equipment::Render()
{
	if(this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CButton_Equipment::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	{
		ModalSprite.textureName = L"T_Button_Equipment";
		ModalSprite.pCreator = this;
		ModalSprite.pRenderer = this->m_pRenderer;
		ModalSprite.pTransform = this->m_pTransform;

		if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
			return E_FAIL;
	}

	/* for. UI Helper Com */
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_UIHelper", L"UIHelper", (CComponent**)&m_pUIHelperCom)))
		return E_FAIL;

	m_pUIHelperCom->UpdateBoundary(m_pLocalTransform);

	return S_OK;
}

_bool CButton_Equipment::ButtonClicked(void)
{
	return m_pUIHelperCom->MouseClickEquipBtn();
}

_int CButton_Equipment::Attach_Owner(void)
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CUI_Indexes*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

CButton_Equipment* CButton_Equipment::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CButton_Equipment* pInstance = new CButton_Equipment(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CButton_Equipment Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton_Equipment::Clone(const _uint iSceneID, void* pArg)
{
	CButton_Equipment* pInstance = new CButton_Equipment(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CButton_Equipment Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CButton_Equipment::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pUIHelperCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}
