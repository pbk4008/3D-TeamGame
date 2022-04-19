#include "pch.h"
#include "UI_LevelUP_Fill_Left.h"
#include "SingleImage.h"
#include "UI_Level_UP.h"
#include "PlayerData.h"

UI_LevelUP_Fill_Left::UI_LevelUP_Fill_Left(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CHud(pDevice, pDeviceContext)
{
}

UI_LevelUP_Fill_Left::UI_LevelUP_Fill_Left(const CHud& rhs)
	: CHud(rhs)
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

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int UI_LevelUP_Fill_Left::Tick(_double dDeltaTime)
{
	if (FAILED(CUI::Tick(dDeltaTime)))
		return -1;

	Bufferdesc.fGapX = m_fGapX = m_fExpRatio;
	Bufferdesc.fGapY = 0.5f;

	if (m_fExExpRatio != m_fExpRatio)
	{
		//Save Pre Data;
		Bufferdesc.fCurExpGauge = m_fCurExpGauge = (m_fExpRatio - m_fExExpRatio);
		m_fExExpRatio = m_fExpRatio;
	}

	return _int();
}

_int UI_LevelUP_Fill_Left::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	m_pSigleImageCom->SetRenderVal(&Bufferdesc);

	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, _vector{ -140.f , -270.f, 0.2f, 1.f });
	m_pLocalTransform->Scaling(_vector{ 230.f, 30.f, 1.f, 0.f });

	Attach_Owner();

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
	{
		CVIBuffer_Trapezium::TRAPDESC desc;
		desc.fAngle = 0.f;
		_tcscpy_s(desc.ShaderFilePath, L"../../Reference/ShaderFile/Shader_UI_Enemy_Bar.hlsl");

		desc.bMinus = true;

		if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Trapezium_UI", L"Com_Trapezium_UI", (CComponent**)&m_pTrapziumBuffer, &desc)))
			return E_FAIL;
	}

	/* for. Single Image Com */
	CSingleImage::Desc ModalSprite;
	ModalSprite.textureName = L"T_HUD_LevelUp_Fill_Left";
	ModalSprite.pCreator = this;
	ModalSprite.pRenderer = this->m_pRenderer;
	ModalSprite.pTransform = this->m_pTransform;
	ModalSprite.fColor = { 1.f, 1.f, 1.f, 0.f };
	ModalSprite.renderType = CSingleImage::VerticalGauge;

	ModalSprite.pBuffer = m_pTrapziumBuffer;
	ModalSprite.fGapX = m_fGapX;
	ModalSprite.fGapY = m_fGapY;
	ModalSprite.fCurExpGauge = m_fCurExpGauge;

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
	m_fExExpRatio = pPlayerData->GetExp() / 100.f;
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
	Safe_Release(m_pSigleImageCom);
	Safe_Release(m_pTrapziumBuffer);
	Safe_Release(m_pLocalTransform);
}
