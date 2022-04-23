#include "pch.h"
#include "UI_LevelUP_Fill_Left.h"
#include "SingleImage.h"
#include "UI_Level_UP.h"
#include "PlayerData.h"

UI_LevelUP_Fill_Left::UI_LevelUP_Fill_Left(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

UI_LevelUP_Fill_Left::UI_LevelUP_Fill_Left(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT UI_LevelUP_Fill_Left::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT UI_LevelUP_Fill_Left::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	desc = (*(Desc*)pArg);

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ -140.f , -270.f, 0.2f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 230.f, 30.f, 1.f, 0.f });

	m_pOwner = desc.pOwner;
	assert(m_pOwner);

	setActive(false);

	Bufferdesc.fExpRatio = 0.f;
	Bufferdesc.fGapX = 0.f;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int UI_LevelUP_Fill_Left::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;


	//if (g_pGameInstance->getkeyPress(DIK_NUMPAD6))
	//{
	//	Bufferdesc.fExpRatio += 0.01f;

	//	if (1.f <= Bufferdesc.fExpRatio)
	//	{
	//		Bufferdesc.fExpRatio = 1.f;
	//	}
	//}
	//if (g_pGameInstance->getkeyPress(DIK_NUMPAD5))
	//{
	//	Bufferdesc.fExpRatio -= 0.01f;

	//	if (0 >= Bufferdesc.fExpRatio)
	//	{
	//		Bufferdesc.fExpRatio = 0.f;
	//	}
	//}

	if (0.f == Bufferdesc.fExpRatio)
	{
		Bufferdesc.fGapX = 0.f;
	}

	if (Bufferdesc.fGapX < Bufferdesc.fExpRatio)
	{
		_float IncreaseGapSpeed = Bufferdesc.fExpRatio -  Bufferdesc.fGapX ;
		Bufferdesc.fGapX += (_float)dDeltaTime * IncreaseGapSpeed * 2.f;
	}

	//Bufferdesc.fGapX = Bufferdesc.fExpRatio;

	return _int();
}

_int UI_LevelUP_Fill_Left::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	m_pSigleImageCom->SetRenderVal(&Bufferdesc);

	Attach_Owner();

	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ -155.f , -270.f, 0.09f, 1.f });

	m_pLocalTransform->Scaling(_vector{ 250.f, 30.f, 1.f, 0.f });

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	return _int();
}

HRESULT UI_LevelUP_Fill_Left::Render()
{
	if (FAILED(CUI::Render()))
		return E_FAIL;

	if (this->getActive())
		m_pSigleImageCom->Render(m_pTransform);
			
	return S_OK;
}

HRESULT UI_LevelUP_Fill_Left::Ready_Component(void)
{

	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Level_Bar", L"Com_Rect_UI_Bar", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_LevelUp_Fill_Left";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.fColor = { 1.f, 1.f, 1.f, 0.f };
	ModalSprite.renderType = CSingleImage::VerticalGaugeLeft;

	ModalSprite.pBuffer = m_pBuffer;
	ModalSprite.fGapX = Bufferdesc.fGapX;
	ModalSprite.fExpRatio = Bufferdesc.fExpRatio;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SingleImage", L"SingleImage", (CComponent**)&m_pSigleImageCom, &ModalSprite)))
		return E_FAIL;

	return S_OK;
}

_int UI_LevelUP_Fill_Left::Attach_Owner()
{
	if (nullptr != m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<UI_LevelUP_Fill_Left*>(m_pOwner)->Get_Transform()->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

void UI_LevelUP_Fill_Left::SetUI(CPlayerData* pPlayerData)
{
	Bufferdesc.fExpRatio = pPlayerData->GetExp() / 100.f;
}
 

UI_LevelUP_Fill_Left* UI_LevelUP_Fill_Left::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UI_LevelUP_Fill_Left* pInstance = new UI_LevelUP_Fill_Left(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("UI_LevelUP_Fill_Left Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* UI_LevelUP_Fill_Left::Clone(const _uint iSceneID, void* pArg)
{
	UI_LevelUP_Fill_Left* pInstance = new UI_LevelUP_Fill_Left(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("UI_LevelUP_Fill_Left Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UI_LevelUP_Fill_Left::Free()
{
	__super::Free();

	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pLocalTransform);
}
