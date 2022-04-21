#include "framework.h"
#include "pch.h"
#include "UI_Tuto_Base.h"
#include "GameInstance.h"

CUI_Tuto_Base::CUI_Tuto_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI_Monster_Panel(pDevice,pDeviceContext)
{
}

CUI_Tuto_Base::CUI_Tuto_Base(const CUI_Monster_Panel& rhs)
	: CUI_Monster_Panel(rhs)
{
}

HRESULT CUI_Tuto_Base::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Tuto_Base::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIACTIVEDESC));
	}

	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}

	m_iObectTag = m_Desc.UIDesc.IDTag;

	if (FAILED(m_pTexture->Change_Texture(m_Desc.UIDesc.TextureTag)))
		return E_FAIL;

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	_vector vPos = { m_Desc.UIDesc.fPos.x - (g_iWinCx >> 1),-m_Desc.UIDesc.fPos.y + (g_iWinCy >> 1), m_Desc.UIDesc.fPos.z,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vScale = { m_Desc.UIDesc.fSize.x,m_Desc.UIDesc.fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);

	return S_OK;
}

_int CUI_Tuto_Base::Tick(_double TimeDelta)
{
	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;

	m_pTransform;

	return 0;
}

_int CUI_Tuto_Base::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		if (g_pInvenUIManager->Get_OpenCheck() == false)
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}
	return _int();
}

HRESULT CUI_Tuto_Base::Render()
{
	/*_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pBuffer->Render(1);*/
	
	return S_OK;
}

HRESULT CUI_Tuto_Base::SetUp_Components()
{
	m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_Rect_UI");

	if (!m_pBuffer)
		return E_FAIL;

	if (FAILED(CGameObject::SetUp_Components(L"Com_Rect_UI", m_pBuffer)))
		return E_FAIL;



	return S_OK;
}

CUI_Tuto_Base* CUI_Tuto_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Tuto_Base* pInstance = new CUI_Tuto_Base(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Tuto_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Tuto_Base::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Tuto_Base* pInstance = new CUI_Tuto_Base(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Tuto_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Tuto_Base::Free()
{
	__super::Free();
}
