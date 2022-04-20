#include "pch.h"
#include "Effect_Dead_Spray.h"
#include "GameInstance.h"


CEffect_Dead_Spray::CEffect_Dead_Spray()
{
	ZeroMemory(&m_tDesc, sizeof(m_tDesc));
}

CEffect_Dead_Spray::CEffect_Dead_Spray(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    :CEffect(pDevice,pDeviceContext)
{
	ZeroMemory(&m_tDesc, sizeof(m_tDesc));
}

CEffect_Dead_Spray::CEffect_Dead_Spray(const CEffect_Dead_Spray& rhs)
	:CEffect(rhs)
	, m_tDesc(rhs.m_tDesc)
{
}

HRESULT CEffect_Dead_Spray::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

    return S_OK;
}

HRESULT CEffect_Dead_Spray::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}

	//if (nullptr != pArg)
	//{
	//	memcpy(&m_Desc, pArg, sizeof(EFFECTDESC));
	//}
	if (pArg)
		m_tDesc = (*(IMAGEEFFECTDESC*)pArg);

	//여기서 필요한 모든 컴포넌트들 Clone해옴
	if (FAILED(SetUp_Components())) 
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CEffect_Dead_Spray::Tick(_double TimeDelta)
{
	_uint iAllFrameCount = (m_Desc.iImageCountX * m_Desc.iImageCountY);
	
	m_Desc.fFrame += (_float)(iAllFrameCount * g_fImmutableTime *m_Desc.fEffectPlaySpeed); //플레이속도 

	if (m_Desc.fFrame >= iAllFrameCount)
	{
		m_Desc.fFrame = 0;
		setActive(false);
	}
	//_vector Pos = { 0.f,0.f,0.f, 1.f };
	//m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);

	//m_pTransform->SetUp_Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(90.f));

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);

	//ViewMatrix *= m_fSize;
	_float fSize = m_tDesc.fSize;
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);
	m_pTransform->Scaling(XMVectorSet(fSize, fSize, fSize, 0.f));

    return 0;
}

_int CEffect_Dead_Spray::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHA, this);
	}

	return 0;
}

HRESULT CEffect_Dead_Spray::Render()
{
	//_matrix XMWorldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix));
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector CamPos = g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	//_uint DebugValue = 4;
	//_uint DebugValue2 = 2;
	m_pBuffer->SetUp_ValueOnShader("g_iImageCountX", &m_Desc.iImageCountX, sizeof(_uint));
	m_pBuffer->SetUp_ValueOnShader("g_iImageCountY", &m_Desc.iImageCountY, sizeof(_uint));

	//m_pBuffer->SetUp_ValueOnShader("g_iImageCountX", &DebugValue, sizeof(_uint));
	//m_pBuffer->SetUp_ValueOnShader("g_iImageCountY", &DebugValue2, sizeof(_uint));


	_float Alpha = 0.8f;
	m_pBuffer->SetUp_ValueOnShader("g_fAlpha", &m_tDesc.fAlpha, sizeof(_float));

	_uint iFrame = (_uint)m_Desc.fFrame;

	m_pBuffer->SetUp_ValueOnShader("g_iFrame", &iFrame, sizeof(_uint));
	
	m_pBuffer->SetUp_ValueOnShader("g_Weight", &m_tDesc.fWeight, sizeof(_float));

	m_pBuffer->Render(1);

	return S_OK;
}

CEffect* CEffect_Dead_Spray::Copy()
{
	CEffect_Dead_Spray* pEffect = new CEffect_Dead_Spray(m_pDevice, m_pDeviceContext);
	if (FAILED(pEffect->NativeConstruct_Prototype()))
	{
		MSGBOX("DashDust Copy Fail");
		Safe_Release(pEffect);
	}
	if (FAILED(pEffect->NativeConstruct(m_iSceneID, &m_tDesc)))
	{
		MSGBOX("HitParticle Copy Fail");
		Safe_Release(pEffect);
	}
	return pEffect;
}

HRESULT CEffect_Dead_Spray::SetUp_Components()
{
	if (!m_pRenderer || !m_pTransform)
		return E_FAIL;

	wstring tag = m_Desc.TextureTag;
	wstring NewTag = L"Texture_" + tag;
	
	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (!m_pTexture)
		return E_FAIL;

	if (FAILED(CGameObject::SetUp_Components(L"Com_Texture", m_pTexture)))
		return E_FAIL;

	if (FAILED(m_pTexture->Change_Texture(NewTag)))
		return E_FAIL;

	_vector vPos = { XMVectorGetX(m_Desc.fMyPos), XMVectorGetY(m_Desc.fMyPos), XMVectorGetZ(m_Desc.fMyPos), 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Effect", L"Com_Rect_Effect", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	return S_OK;
}

CEffect_Dead_Spray* CEffect_Dead_Spray::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CEffect_Dead_Spray* pInstance = new CEffect_Dead_Spray(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CEffect_Dead_Spray");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Dead_Spray::Clone(const _uint _iSceneID, void* pArg)
{
	/* 복제본 생성할때는 아래함수 호출해서 추가 초기화를 진행 */
	CEffect_Dead_Spray* pInstance = new CEffect_Dead_Spray(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CEffect_Dead_Spray");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Dead_Spray::Free()
{
	Safe_Release(m_pBuffer);
	__super::Free();
}
