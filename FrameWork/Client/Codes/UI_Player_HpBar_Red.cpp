#include "framework.h"
#include "pch.h"
#include "UI_Player_HpBar_Red.h"
#include "UI_Player_HpBar.h"
#include "GameInstance.h"

CUI_Player_HpBar_Red::CUI_Player_HpBar_Red(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
}

CUI_Player_HpBar_Red::CUI_Player_HpBar_Red(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_HpBar_Red::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Player_HpBar_Red::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIDESC));
	}
	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}
	
	m_iCurScene = _iSceneID;
	m_iObectTag = m_Desc.IDTag;

	if (FAILED(m_pTexture->Change_Texture(m_Desc.TextureTag)))
		return E_FAIL;

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	_vector vpos = { -400.f, -252.1f, 0.09f, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vpos);

	_vector vScale = { m_Desc.fSize.x,m_Desc.fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);

	m_fGapX = 1.f;
	m_fGapY = 1.f;

	return S_OK;
}

_int CUI_Player_HpBar_Red::Tick(_double TimeDelta)
{
	if (FAILED(__super::Tick(TimeDelta)))
		return -1;

	//초록색으로부터 현재 hp가져오려고 
	CUI_Player_HpBar* pUI = (CUI_Player_HpBar*)g_pGameInstance->getObjectList(m_iCurScene, L"Layer_UI_Green")->front();
	
	if (nullptr != pUI)
	{
		_float fHpRatio = pUI->GetRatio();

		if (1.f == fHpRatio)
		{
			m_fGapX = 1.f;
		}

		if (1.f > fHpRatio)
		{
			//옆에 살짝보여서 추가적으로 더 깎아줌p 
			fHpRatio = pUI->GetRatio() - 0.031f;
		}

		if (m_fGapX > fHpRatio)
		{
			_float DecreaseGapSpeed = m_fGapX - fHpRatio;
			m_fGapX -= (_float)TimeDelta * DecreaseGapSpeed; //데미지가 더 많이 들어오면 더 빨리 깎아주려고
		}
	}

	return 0;
}

_int CUI_Player_HpBar_Red::LateTick(_double TimeDelta)
{
	if (FAILED(__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		if (g_pInvenUIManager->Get_OpenCheck() == false)
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}
	return _int();
}

HRESULT CUI_Player_HpBar_Red::Render()
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pTrapziumBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_fX", &m_fGapX, sizeof(_float));
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_fY", &m_fGapY, sizeof(_float));

	m_pTrapziumBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pTrapziumBuffer->Render(1);

	return S_OK;
}

HRESULT CUI_Player_HpBar_Red::SetUp_Components()
{
	CVIBuffer_Trapezium::TRAPDESC Desc;
	Desc.fAngle = m_Desc.fAngle;
	_tcscpy_s(Desc.ShaderFilePath, L"../../Reference/ShaderFile/Shader_UI_Bar.hlsl");

	Desc.bMinus = m_Desc.bMinus;

	m_pTrapziumBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Trapezium>(0, L"Proto_Component_Trapezium_UI", &Desc);

	if (!m_pTrapziumBuffer)
		return E_FAIL;

	if (FAILED(CGameObject::SetUp_Components(L"Com_Trapezium_UI", m_pTrapziumBuffer)))
		return E_FAIL;

	return S_OK;
}

CUI_Player_HpBar_Red* CUI_Player_HpBar_Red::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Player_HpBar_Red* pInstance = new CUI_Player_HpBar_Red(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Player_HpBar_Red");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Player_HpBar_Red::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Player_HpBar_Red* pInstance = new CUI_Player_HpBar_Red(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Player_HpBar_Red");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Player_HpBar_Red::Free()
{
	__super::Free();
}
