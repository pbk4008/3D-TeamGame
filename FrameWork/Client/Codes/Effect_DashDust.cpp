#include "pch.h"
#include "Effect_DashDust.h"
#include "GameInstance.h"
#include "VIBuffer_PointInstance_Explosion.h"


CEffect_DashDust::CEffect_DashDust()
{
}

CEffect_DashDust::CEffect_DashDust(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    :CEffect(pDevice,pDeviceContext)
{
}

CEffect_DashDust::CEffect_DashDust(const CEffect& rhs)
    :CEffect(rhs)
{
}

HRESULT CEffect_DashDust::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

    return S_OK;
}

HRESULT CEffect_DashDust::NativeConstruct(const _uint _iSceneID, void* pArg)
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

	CVIBuffer_PointInstance_Floating_Disappear::PIDESC Desc;
	_tcscpy_s(Desc.ShaderFilePath, m_Desc.ShaderFullFilePath);
	Desc.matParticle = m_Desc.ParticleMat;
	Desc.fParticleStartRandomPos = m_Desc.fParticleRandomPos;
	Desc.fParticleMinusRandomDir = m_Desc.fParticleMinusRandomDir;
	Desc.fParticleRandomDir = m_Desc.fParticleRandomDir;
	Desc.fParticleSpeed = m_Desc.fParticleVelocity;
	Desc.fParticleSize = m_Desc.fParticleSize;
	Desc.iNumInstance = m_Desc.iNumInstance;
	Desc.fLifeTime = m_Desc.fMaxLifeTime;
	Desc.fCurTime = m_Desc.fCurTime;
	Desc.bGravity = m_Desc.bUsingGravity;

	//m_pBuffer->Set_Desc(Desc);
	//m_pBuffer->Particle_Reset();

	m_fAlpha = 0.2f;

	m_backupDesc = Desc;

	setActive(false);

	return S_OK;
}

_int CEffect_DashDust::Tick(_double TimeDelta)
{
	m_pBuffer->Update(TimeDelta, m_Desc.iAxis);

	m_fNonActiveTimeAcc += (_float)g_dImmutableTime;
	if (4.f <= m_fNonActiveTimeAcc)
	{
		setActive(false);
		m_fNonActiveTimeAcc = 0.f;
	}

	_matrix matCullingBoxPivot = XMMatrixIdentity();
	matCullingBoxPivot.r[3] = { m_Desc.CullingBoxPos.x, m_Desc.CullingBoxPos.y,m_Desc.CullingBoxPos.z, 1.f };
	m_pBox->Update_Matrix(matCullingBoxPivot * m_pTransform->Get_WorldMatrix());

	//이미지 플레이
	_uint iAllFrameCount = (m_Desc.iImageCountX * m_Desc.iImageCountY);
	m_Desc.fFrame += (_float)(iAllFrameCount * TimeDelta * m_Desc.fEffectPlaySpeed); //플레이속도 
	if (m_Desc.fFrame >= iAllFrameCount)
	{
		m_Desc.fFrame = 0;
	}

	if (m_Desc.fMaxLifeTime > m_Desc.fCurTime)
	{
		m_Desc.fCurTime += (_float)TimeDelta;
	}

	if (m_Desc.fMaxLifeTime < m_Desc.fCurTime)
	{
		m_Desc.fCurTime = m_Desc.fMaxLifeTime;
	}

	m_fAlpha -= (_float)TimeDelta;

	if (0 >= m_fAlpha)
	{
		setActive(false);
	}

    return 0;
}

_int CEffect_DashDust::LateTick(_double TimeDelta)
{
	_bool bCulling = g_pGameInstance->isIn_WorldFrustum(m_pBox->Get_Points(), 1.f);
	if (true == bCulling)
	{ 
		if (nullptr != m_pRenderer)
		{
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHA, this);
		}
	}
	return 0;
}

HRESULT CEffect_DashDust::Render()
{
	//m_pBox->Render(L"Camera_Silvermane");
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
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

	m_pBuffer->SetUp_ValueOnShader("g_fLifeTime", &m_Desc.fMaxLifeTime, sizeof(_float));
	m_pBuffer->SetUp_ValueOnShader("g_fCurTime", &m_Desc.fCurTime, sizeof(_float));

	m_pBuffer->SetUp_ValueOnShader("g_fAlpha", &m_fAlpha, sizeof(_float));
	_float power = 1.f;
	m_pBuffer->SetUp_ValueOnShader("g_empower", &power, sizeof(_float));

	m_pBuffer->SetUp_ValueOnShader("g_vCamPosition", (void*)&CamPos, sizeof(_vector));

	m_pBuffer->Render(m_Desc.iRenderPassNum);

	return S_OK;
}

CEffect* CEffect_DashDust::Copy()
{
	CEffect_DashDust* pEffect = new CEffect_DashDust(m_pDevice, m_pDeviceContext);
	if (FAILED(pEffect->NativeConstruct_Prototype()))
	{
		MSGBOX("DashDust Copy Fail");
		Safe_Release(pEffect);
	}
	if (FAILED(pEffect->NativeConstruct(m_iSceneID, &m_Desc)))
	{
		MSGBOX("HitParticle Copy Fail");
		Safe_Release(pEffect);
	}

	return pEffect;
}

void CEffect_DashDust::Set_Reset(_bool bReset)
{
	CEffect::Set_Reset(bReset);
	m_Desc.fCurTime = 0.f;
	m_fAlpha = 0.5f;
	m_pBuffer->Set_Desc(m_backupDesc);
	m_pBuffer->Particle_Reset();
}

HRESULT CEffect_DashDust::SetUp_Components()
{
	if (!m_pTexture || !m_pRenderer || !m_pTransform)
		return E_FAIL;

	//이미지 리스트박스로부터 가져옴
	wstring tag = m_Desc.TextureTag;
	wstring NewTag = L"Texture_" + tag;
	if (FAILED(m_pTexture->Change_Texture(NewTag)))
		return E_FAIL;

	_vector vPos = { /*XMVectorGetX(m_Desc.fMyPos), XMVectorGetY(m_Desc.fMyPos), XMVectorGetZ(m_Desc.fMyPos)*/0,1.5f,0, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	//culling 
	m_pBox = g_pGameInstance->Clone_Component<CCullingBox>(0, L"Proto_Component_CullingBox");
	if (!m_pBox)
		return E_FAIL;
	m_pBox->Set_Length(m_Desc.CullingBoxSize.x, m_Desc.CullingBoxSize.y, m_Desc.CullingBoxSize.z);

	//버퍼 Clone
	_tcscpy_s(m_backupDesc.ShaderFilePath, m_Desc.ShaderFullFilePath);
	m_backupDesc.matParticle = m_Desc.ParticleMat;
	m_backupDesc.fParticleStartRandomPos = m_Desc.fParticleRandomPos;
	m_backupDesc.fParticleMinusRandomDir = m_Desc.fParticleMinusRandomDir;
	m_backupDesc.fParticleRandomDir = m_Desc.fParticleRandomDir;
	m_backupDesc.fParticleSpeed = m_Desc.fParticleVelocity;
	m_backupDesc.fParticleSize = m_Desc.fParticleSize;
	m_backupDesc.iNumInstance = m_Desc.iNumInstance;
	m_backupDesc.fLifeTime = m_Desc.fMaxLifeTime;
	m_backupDesc.fCurTime = m_Desc.fCurTime;
	m_backupDesc.bGravity = m_Desc.bUsingGravity;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_PointInstance_Disappear", L"Com_VIBuffer", (CComponent**)&m_pBuffer, &m_backupDesc)))
		return E_FAIL;

	return S_OK;
}

CEffect_DashDust* CEffect_DashDust::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CEffect_DashDust* pInstance = new CEffect_DashDust(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CEffect_DashDust");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_DashDust::Clone(const _uint _iSceneID, void* pArg)
{
	/* 복제본 생성할때는 아래함수 호출해서 추가 초기화를 진행 */
	CEffect_DashDust* pInstance = new CEffect_DashDust(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CEffect_DashDust");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_DashDust::Free()
{
	Safe_Release(m_pBox);
	Safe_Release(m_pBuffer);

	__super::Free();
}
