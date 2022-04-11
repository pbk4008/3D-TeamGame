#include "pch.h"
#include "Button_Skill.h"
#include "SingleImage.h"
#include "UIHelper.h"

CButton_Skill::CButton_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CButton_Skill::CButton_Skill(const CButton_Skill& rhs)
	: CUI(rhs)
{
}

HRESULT CButton_Skill::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CButton_Skill::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ desc.fPos.x, desc.fPos.y, 0.4f, 1.f });
	m_pTransform->Scaling(_vector{ desc.fScale.x, desc.fScale.y, 1.f, 1.f });

	setActive(false);

	return S_OK;
}

_int CButton_Skill::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CButton_Skill::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}

	return _int();
}

HRESULT CButton_Skill::Render()
{
	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CButton_Skill::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_Item_Bg_None";
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

_bool CButton_Skill::ButtonClicked(void)
{
	return m_pUIHelperCom->MouseClickDown();
}

CButton_Skill* CButton_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CButton_Skill* pInstance = new CButton_Skill(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CButton_Skill Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton_Skill::Clone(const _uint iSceneID, void* pArg)
{
	CButton_Skill* pInstance = new CButton_Skill(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CButton_Skill Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CButton_Skill::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pUIHelperCom);
	__super::Free();
}
