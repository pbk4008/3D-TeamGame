#include "pch.h"
#include "Effect_Env_Fire.h"
#include "GameInstance.h"
#include "Light.h"

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
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
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

	if (m_Desc.IDTag != (_uint)SCENEID::SCENE_STAGE1)
	{
		LIGHTDESC LightDesc;
		ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
		LightDesc.eType = LIGHTDESC::TYPE_POINT;
		LightDesc.fRange = 3.f;
		LightDesc.vDiffuse = _float4(1.0f, 0.34509f, 0.1333f, 1.f);
		LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 1.f);
		LightDesc.vAmbient = _float4(0.8f, 0.8f, 0.8f, 1.f);
		LightDesc.bactive = true;
		LightDesc.vPosition = _float3(desc._41, desc._42 + 2.5f, desc._43);

		m_range = LightDesc.fRange;

		if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc, &m_plight))) MSGBOX("Failed To Adding PointLight");
	}

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

	_vector pos = m_Desc.ParticleMat.r[3];


	if (L"Texture_fx_smoke_a" == m_pTexture->getTextureTag())
	{
		_vector svPos = m_Desc.ParticleMat.r[3];
		_vector svLook = m_Desc.ParticleMat.r[2];

		svPos = XMVectorSetY(svPos, XMVectorGetY(svPos) + 0.0f);
		pos = svPos + svLook * 0.f;
	}
	

	_vector pos2 = XMVectorZero();
	if (m_Desc.bUsingGravity == true)
		pos2 = { XMVectorGetX(pos),XMVectorGetY(pos) + (XMVectorGetY(m_scale) * 5.f) ,XMVectorGetZ(pos) ,1.f };
	else
		pos2 = { XMVectorGetX(pos),XMVectorGetY(pos) + (XMVectorGetY(m_scale)) ,XMVectorGetZ(pos) ,1.f };

	m_pTransform->Set_State(CTransform::STATE_POSITION, pos2);

	//빌보드
	_matrix ViewMatrix;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	ViewMatrix = g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	if (m_Desc.bUsingGravity == true)
		m_pTransform->Scaling(m_scale * 7.f);
	else
		m_pTransform->Scaling(m_scale * 1.f);


	if (m_plight)
	{
		m_plight->Set_Pos(m_pTransform->Get_State(CTransform::STATE_POSITION) - XMVectorSet(0.f,1.f,0.f,0.f));
		m_range += g_fDeltaTime * 2.f * m_param;

		if (m_range < 3.f)
		{
			m_param *= -1.f;
			m_range = 3.f;
		}
		else if (m_range > 4.f)
		{
			m_param *= -1.f;
			m_range = 4.f;
		}

		m_plight->Set_Range(m_range);
	}

    return 0;
}

_int CEffect_Env_Fire::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer && g_pGameInstance->isIn_WorldFrustum(m_pTransform->Get_State(CTransform::STATE_POSITION), 5.f) == true)
	{
		if (L"Texture_fx_smoke_a" == m_pTexture->getTextureTag())
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHANB, this);
		else
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHA, this);
	}

	return 0;
}

HRESULT CEffect_Env_Fire::Render()
{
	//_matrix XMWorldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix));
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector CamPos = g_pGameInstance->Get_CamPosition(wstrCamTag);

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pBuffer->SetUp_ValueOnShader("g_iImageCountX", &m_Desc.iImageCountX, sizeof(_uint));
	m_pBuffer->SetUp_ValueOnShader("g_iImageCountY", &m_Desc.iImageCountY, sizeof(_uint));

	_uint iFrame = (_uint)m_Desc.fFrame;
	m_pBuffer->SetUp_ValueOnShader("g_iFrame", &iFrame, sizeof(_uint));
	
	if (L"Texture_fx_smoke_a" == m_pTexture->getTextureTag())
	{
		_float weight = 0.f;
		m_pBuffer->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float));
	}
	else
	{
		_float weight = 0.8f;
		m_pBuffer->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float));
	}
	
	m_pBuffer->Render(1);

	return S_OK;
}

HRESULT CEffect_Env_Fire::SetUp_Components()
{
	if (!m_pRenderer || !m_pTransform)
		return E_FAIL;

	//이미지 리스트박스로부터 가져옴
	wstring tag = m_Desc.TextureTag;
	wstring NewTag = L"Texture_" + tag;

	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Texture", L"Com_Texture", (CComponent**)&m_pTexture)))
		return E_FAIL;

	if (FAILED(m_pTexture->Change_Texture(NewTag)))
		return E_FAIL;

	_vector vPos = { XMVectorGetX(m_Desc.fMyPos), XMVectorGetY(m_Desc.fMyPos), XMVectorGetZ(m_Desc.fMyPos), 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Effect", L"Com_Rect_Effect", (CComponent**)&m_pBuffer)))
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
