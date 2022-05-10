#include "pch.h"
#include "Quest.h"
#include "UI_QuestText.h"
#include "SingleImage.h"

CUI_QuestText::CUI_QuestText(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_QuestText::CUI_QuestText(const CUI_QuestText& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_QuestText::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_QuestText::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);
	
	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pOwner = desc.pOwner;
	assert(m_pOwner);

	setActive(false);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI_QuestText::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	return _int();
}

_int CUI_QuestText::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	Attach_Owner();
	
	//m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ -710.f, 270.f, 0.02f, 1.f });
	//m_pTransform->Scaling(_vector{ 400.f , 43.f, 1.f, 0.f });

	return _int();
}

HRESULT CUI_QuestText::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

 	m_pSigleImageCom->Render(m_pTransform);

	return S_OK;
}

HRESULT CUI_QuestText::Ready_Component(void)
{
	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = g_arrQuestTextTex[(_int)desc.EQuestText];
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.bFadeOption = true;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

_int CUI_QuestText::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CQuest*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

void CUI_QuestText::SetIcon(std::wstring _szTextureName)
{
	m_pSigleImageCom->SetTexture(_szTextureName);
}

void CUI_QuestText::SetPosy(_float fPosy)
{
	m_fPosY -= fPosy;

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ -710.f, m_fPosY, 0.03f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 400.f, 43.f, 1.f, 0.f });
}

void CUI_QuestText::SetFadeOut(void)
{
	m_pSigleImageCom->SetFadeOut();
}

CUI_QuestText* CUI_QuestText::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_QuestText* pInstance = new CUI_QuestText(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CUI_QuestHeadText Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_QuestText::Clone(const _uint iSceneID, void* pArg)
{
	CUI_QuestText* pInstance = new CUI_QuestText(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CUI_QuestHeadText Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_QuestText::Free()
{
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}
