#include "pch.h"
#include "IndexIndicator.h"
#include "SingleImage.h"
#include "UIHelper.h"

CIndexIndicator::CIndexIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CIndexIndicator::CIndexIndicator(const CIndexIndicator& rhs)
	: CUI(rhs)
{
}

HRESULT CIndexIndicator::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CIndexIndicator::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ -815.f, 410.f, 0.4f, 1.f });
	m_pTransform->Scaling(_vector{ desc.fScale.x - 50.f, desc.fScale.y - 50.f, 1.f, 1.f });

	setActive(false);

	return S_OK;
}

_int CIndexIndicator::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CIndexIndicator::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (!m_bMove)
		MoveLeft(TimeDelta);
	else
		MoveRight(TimeDelta);

	return _int();
}

HRESULT CIndexIndicator::Render()
{
	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CIndexIndicator::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_Indicator";
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

_bool CIndexIndicator::ButtonClicked(void)
{
	return m_pUIHelperCom->MouseClickDown();
}

void CIndexIndicator::ResetXPos(void)
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ -815.f, 410.f, 0.4f, 1.f });

	m_fEndPos  = -820.f;
	m_fInitPos = -720.f;
}

void CIndexIndicator::MoveLeft(_double TimeDelta)
{
	if (m_fInitPos > m_fEndPos)
	{
		m_fInitPos -= (_float)TimeDelta * 1500.f;

		if (m_fInitPos < m_fEndPos)
		{
			m_fInitPos = m_fEndPos;
		}
		m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ m_fInitPos, 410.f, 0.2f, 1.f });
	}
}

void CIndexIndicator::MoveRight(_double TimeDelta)
{
	if (m_fInitPos < m_fEndPos)
	{
		m_fInitPos += (_float)TimeDelta * 1500.f;
		if (m_fInitPos > m_fEndPos)
		{
			m_fInitPos = m_fEndPos;
		}
		m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ m_fInitPos, 410.f, 0.3f, 1.f });
	}
}

CIndexIndicator* CIndexIndicator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CIndexIndicator* pInstance = new CIndexIndicator(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CIndexIndicator Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIndexIndicator::Clone(const _uint iSceneID, void* pArg)
{
	CIndexIndicator* pInstance = new CIndexIndicator(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CIndexIndicator Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CIndexIndicator::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pUIHelperCom);
	__super::Free();
}
