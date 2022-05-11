#include "pch.h"
#include "Quest.h"
#include "UI_QuestClear.h"
#include "SingleImage.h"

CUI_QuestClear::CUI_QuestClear(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_QuestClear::CUI_QuestClear(const CUI_QuestClear& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_QuestClear::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CUI_QuestClear::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);
	
	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	
	XMStoreFloat4(&m_fOwnerPos, desc.pQuestTextTrans->Get_State(CTransform::STATE_POSITION));

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ -894.f, desc.fInitPosY, 0.02f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 40.5f , 40.5f, 1.f, 0.f });

	m_pOwner = desc.pOwner;
	assert(m_pOwner);

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_QuestClear::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_QuestClear::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	//m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	if (!m_bShowEnd)
		Show(TimeDelta);

//	if(m_bShowEnd)
		Attach_Owner();

	return _int();
}

HRESULT CUI_QuestClear::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

 	m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}


void CUI_QuestClear::Show(_double dTimeDelta)
{
	if (m_fInitScale.x > m_fEndScale.x)
	{
		m_fInitScale.y = m_fInitScale.x -= (_float)dTimeDelta * 500.f;

		if (m_fInitScale.x <= m_fEndScale.x)
		{
			m_fInitScale.x = m_fEndScale.x;
			m_fInitScale.y = m_fEndScale.y;
			m_bShowEnd = true;
		}
		m_pLocalTransform->Scaling(_vector{ m_fInitScale.x , m_fInitScale.y, 1.f, 0.f });
	}

}
_int CUI_QuestClear::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CQuest*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}


HRESULT CUI_QuestClear::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_QuestClearMark";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	//ModalSprite.bFadeOption = true;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;
	m_pSigleImageCom->SetRenderPass(4);
	
	return S_OK;
}

void CUI_QuestClear::SetIcon(std::wstring _szTextureName)
{
	m_pSigleImageCom->SetTexture(_szTextureName);
}

void CUI_QuestClear::SetPosy(_float fPosy)
{
	m_fPosY -= fPosy;

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ -894.f, m_fPosY + 2.f, 0.02f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 40.5f , 40.5f, 1.f, 0.f });
}

void CUI_QuestClear::SetFadeOut(void)
{
	m_pSigleImageCom->SetFadeOut();
}

void CUI_QuestClear::PlusYPos(void)
{

}

CUI_QuestClear* CUI_QuestClear::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_QuestClear* pInstance = new CUI_QuestClear(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_QuestHeadText Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_QuestClear::Clone(const _uint iSceneID, void* pArg)
{
	CUI_QuestClear* pInstance = new CUI_QuestClear(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_QuestHeadText Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_QuestClear::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}
