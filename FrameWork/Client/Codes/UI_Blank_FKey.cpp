#include "framework.h"
#include "pch.h"
#include "UI_Blank_FKey.h"
#include "GameInstance.h"
#include "Silvermane.h"

CUI_Blank_FKey::CUI_Blank_FKey(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

CUI_Blank_FKey::CUI_Blank_FKey(const CUI_Blank_FKey& rhs)
	: CUI(rhs)
	, m_Desc(rhs.m_Desc)
{
}

HRESULT CUI_Blank_FKey::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Blank_FKey::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIACTIVEDESC));
	}

	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}
	m_iSceneID = _iSceneID;

	m_iObectTag = m_Desc.UIDesc.IDTag;

	if (FAILED(m_pTexture->Change_Texture(m_Desc.UIDesc.TextureTag)))
		return E_FAIL;

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	_vector vScale = { m_Desc.UIDesc.fSize.x,m_Desc.UIDesc.fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);

	setActive(false);

	return S_OK;
}

_int CUI_Blank_FKey::Tick(_double TimeDelta)
{
	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;

	return 0;
}

_int CUI_Blank_FKey::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_UP, ViewMatrix.r[1]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();
	vScale = { fDistToUI * 0.08f,fDistToUI * 0.08f,1.f,1.f };
	m_pTransform->Scaling(vScale);

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}
	return _int();
}

HRESULT CUI_Blank_FKey::Render()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pBuffer->Render(1);

	return S_OK;
}

void CUI_Blank_FKey::Set_Position(const _fvector& _svPos)
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(_svPos, 1.f));
}

HRESULT CUI_Blank_FKey::SetUp_Components()
{
	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_UI", L"Com_Rect_UI", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	return S_OK;
}

CUI_Blank_FKey* CUI_Blank_FKey::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Blank_FKey* pInstance = new CUI_Blank_FKey(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Blank_FKey");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Blank_FKey::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Blank_FKey* pInstance = new CUI_Blank_FKey(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Blank_FKey");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Blank_FKey::Free()
{
	__super::Free();
}
