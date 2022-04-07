#include "pch.h"
#include "Effect_Env_Fire.h"
#include "GameInstance.h"


CEffect_Env_Fire::CEffect_Env_Fire()
{
}

CEffect_Env_Fire::CEffect_Env_Fire(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    :CEffect(pDevice,pDeviceContext)
{
}

CEffect_Env_Fire::CEffect_Env_Fire(const CEffect& rhs)
    :CEffect(rhs)
{
}

HRESULT CEffect_Env_Fire::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

    return S_OK;
}

HRESULT CEffect_Env_Fire::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}

	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(EFFECTDESC));
	}
	//여기서 필요한 모든 컴포넌트들 Clone해옴
	if (FAILED(SetUp_Components())) 
	{
		return E_FAIL;
	}

	_float4x4 desc;
	XMStoreFloat4x4(&desc,m_Desc.ParticleMat);
	_vector pos = m_Desc.ParticleMat.r[3];
	pos += XMVectorSet(0, 2.5f, 0, 0);

	m_pTransform->Set_State(CTransform::STATE_POSITION, pos);


	m_scale = XMVectorSet(XMVectorGetX(XMVector3Length(m_Desc.ParticleMat.r[0]))
			, XMVectorGetX(XMVector3Length(m_Desc.ParticleMat.r[1]))
			, XMVectorGetX(XMVector3Length(m_Desc.ParticleMat.r[2]))
			, 1);

	return S_OK;
}

_int CEffect_Env_Fire::Tick(_double TimeDelta)
{
	
	_uint iAllFrameCount = (m_Desc.iImageCountX * m_Desc.iImageCountY);
	m_Desc.fFrame += (_float)(iAllFrameCount * TimeDelta * /*m_Desc.fEffectPlaySpeed*/ 1); //플레이속도 
	if (m_Desc.fFrame >= iAllFrameCount)
	{
		m_Desc.fFrame = 0;
	}

	////_vector Pos = { 9.5f,4.f,6.5f, 1.f };
	//m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);

	_vector pos = m_Desc.ParticleMat.r[3];
	_vector pos2 = { XMVectorGetX(pos),XMVectorGetY(pos) + (XMVectorGetY(m_scale) * 5.f) ,XMVectorGetZ(pos) ,1.f };

	m_pTransform->Set_State(CTransform::STATE_POSITION, pos2);

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	//_vector  scale = { 3.f, 3.f, 1.f, 0.f };
	m_pTransform->Scaling(m_scale * 7.f);

    return 0;
}

_int CEffect_Env_Fire::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHA, this);
	}

	return 0;
}

HRESULT CEffect_Env_Fire::Render()
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

	m_pBuffer->SetUp_ValueOnShader("g_iImageCountX", &m_Desc.iImageCountX, sizeof(_uint));
	m_pBuffer->SetUp_ValueOnShader("g_iImageCountY", &m_Desc.iImageCountY, sizeof(_uint));

	_uint iFrame = (_uint)m_Desc.fFrame;
	m_pBuffer->SetUp_ValueOnShader("g_iFrame", &iFrame, sizeof(_uint));
	
	_float weight = 0.8f;
	m_pBuffer->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float));

	m_pBuffer->Render(1);

	return S_OK;
}

HRESULT CEffect_Env_Fire::SetUp_Components()
{
	if (!m_pTexture || !m_pRenderer || !m_pTransform)
		return E_FAIL;

	//이미지 리스트박스로부터 가져옴
	wstring tag = m_Desc.TextureTag;
	wstring NewTag = L"Texture_" + tag;
	if (FAILED(m_pTexture->Change_Texture(NewTag)))
		return E_FAIL;

	_vector vPos = { XMVectorGetX(m_Desc.fMyPos), XMVectorGetY(m_Desc.fMyPos), XMVectorGetZ(m_Desc.fMyPos), 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Proto_Component_Rect_Effect", L"Com_Rect_Effect", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	return S_OK;
}

CEffect_Env_Fire* CEffect_Env_Fire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CEffect_Env_Fire* pInstance = new CEffect_Env_Fire(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CEffect_Env_Fire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Env_Fire::Clone(const _uint _iSceneID, void* pArg)
{
	/* 복제본 생성할때는 아래함수 호출해서 추가 초기화를 진행 */
	CEffect_Env_Fire* pInstance = new CEffect_Env_Fire(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CEffect_Env_Fire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Env_Fire::Free()
{
	Safe_Release(m_pBuffer);
	__super::Free();
}
