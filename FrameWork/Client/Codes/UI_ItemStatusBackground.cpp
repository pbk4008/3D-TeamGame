#include "pch.h"
#include "UI_ItemStatusBackground.h"
#include "SingleImage.h"

UI_ItemStatusBackground::UI_ItemStatusBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

UI_ItemStatusBackground::UI_ItemStatusBackground(const UI_ItemStatusBackground& rhs)
	: CUI(rhs)
{
}

HRESULT UI_ItemStatusBackground::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT UI_ItemStatusBackground::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ m_fInitPos, 0.f, 0.3f, 1.f });
	//m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 420.f, 0.f, 0.3f, 1.f });
	m_pTransform->Scaling(_vector{ 360.f, 580.f, 1.f, 1.f });

	setActive(true);

	return S_OK;
}

_int UI_ItemStatusBackground::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;



	return _int();
}

_int UI_ItemStatusBackground::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (m_fInitPos > m_fEndPos)
	{
		m_fInitPos -= (_float)TimeDelta * 100.f;
		if (m_fInitPos < m_fEndPos)
		{
			m_fInitPos = m_fEndPos;
		}
		m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ m_fInitPos, 0.f, 0.3f, 1.f });
	}
	
	return _int();
}

HRESULT UI_ItemStatusBackground::Render()
{
	if (FAILED(CUI::Render()))
		return -1;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

void UI_ItemStatusBackground::setActive(_bool bActive)
{
	this->m_bActive = bActive;
	
	if (false == bActive)
		m_fInitPos = 430.f;
}

HRESULT UI_ItemStatusBackground::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_ItemStatus_Bg_Common";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

CSingleImage* UI_ItemStatusBackground::GetSingleImage(void)
{
	return m_pSigleImageCom;
}

UI_ItemStatusBackground* UI_ItemStatusBackground::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UI_ItemStatusBackground* pInstance = new UI_ItemStatusBackground(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("UI_ItemStatusBackground Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* UI_ItemStatusBackground::Clone(const _uint iSceneID, void* pArg)
{
	UI_ItemStatusBackground* pInstance = new UI_ItemStatusBackground(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("UI_ItemStatusBackground Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UI_ItemStatusBackground::Free()
{
	Safe_Release(m_pSigleImageCom);

	__super::Free();
}
