#include "pch.h"
#include "Effect_Hammer_Dust.h"
#include "GameInstance.h"


CEffect_Hammer_Dust::CEffect_Hammer_Dust()
{
}

CEffect_Hammer_Dust::CEffect_Hammer_Dust(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CEffect(pDevice, pDeviceContext)
{
}

CEffect_Hammer_Dust::CEffect_Hammer_Dust(const CEffect& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Hammer_Dust::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Hammer_Dust::NativeConstruct(const _uint _iSceneID, void* pArg)
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

	setActive(true);

	return S_OK;
}

_int CEffect_Hammer_Dust::Tick(_double TimeDelta)
{

	_uint iAllFrameCount = (m_Desc.iImageCountX * m_Desc.iImageCountY);
	
	m_Desc.fFrame += (_float)(iAllFrameCount * TimeDelta * /*m_Desc.fEffectPlaySpeed*/1.7f); //플레이속도 

	if (m_Desc.fFrame >= iAllFrameCount)
	{
		m_Desc.fFrame = 0;
		//setActive(false);
	}


	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);

	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();

	vScale = { 1.f, 1.f,1.f,0.f };
	m_pTransform->Scaling(vScale);


	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 2.f, 0.f, 1.f));
    return 0;

}

_int CEffect_Hammer_Dust::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHANB, this);
	}

	return 0;
}

HRESULT CEffect_Hammer_Dust::Render()
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

	m_Alpha = 1.f;
	m_pBuffer->SetUp_ValueOnShader("g_fAlpha", &m_Alpha, sizeof(_float));

	m_pBuffer->Render(3);

	return S_OK;
}

CEffect* CEffect_Hammer_Dust::Copy()
{
	CEffect_Hammer_Dust* pEffect = new CEffect_Hammer_Dust(m_pDevice, m_pDeviceContext);
	if (FAILED(pEffect->NativeConstruct_Prototype()))
	{
		MSGBOX("Hammer_Dust Copy Fail");
		Safe_Release(pEffect);
	}
	if (FAILED(pEffect->NativeConstruct(m_iSceneID, &m_Desc)))
	{
		MSGBOX("Hammer_Dust Copy Fail");
		Safe_Release(pEffect);
	}

	return pEffect;
}

void CEffect_Hammer_Dust::Set_Reset(_bool bReset)
{
	//setActive(false);
}

HRESULT CEffect_Hammer_Dust::SetUp_Components()
{
	if (!m_pTexture || !m_pRenderer || !m_pTransform)
		return E_FAIL;

	wstring tag = m_Desc.TextureTag;
	wstring NewTag = L"Texture_" + tag;
	if (FAILED(m_pTexture->Change_Texture(NewTag)))
		return E_FAIL;

	_vector vPos = { XMVectorGetX(m_Desc.fMyPos), XMVectorGetY(m_Desc.fMyPos), XMVectorGetZ(m_Desc.fMyPos), 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Effect", L"Com_Rect_Effect", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	return S_OK;
}

CEffect_Hammer_Dust* CEffect_Hammer_Dust::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CEffect_Hammer_Dust* pInstance = new CEffect_Hammer_Dust(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CEffect_Hammer_Dust");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Hammer_Dust::Clone(const _uint _iSceneID, void* pArg)
{
	/* 복제본 생성할때는 아래함수 호출해서 추가 초기화를 진행 */
	CEffect_Hammer_Dust* pInstance = new CEffect_Hammer_Dust(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CEffect_Hammer_Dust");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Hammer_Dust::Free()
{
	__super::Free();
	Safe_Release(m_pBuffer);
}