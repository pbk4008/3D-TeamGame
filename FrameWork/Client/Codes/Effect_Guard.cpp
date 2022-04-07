#include "pch.h"
#include "Effect_Guard.h"
#include "GameInstance.h"
#include "VIBuffer_PointInstance_Explosion.h"


CEffect_Guard::CEffect_Guard()
{
}

CEffect_Guard::CEffect_Guard(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    :CEffect(pDevice,pDeviceContext)
{
}

CEffect_Guard::CEffect_Guard(const CEffect_Guard& rhs)
    :CEffect(rhs)
	, m_Desc(rhs.m_Desc)
	, m_backupDesc(rhs.m_backupDesc)
{
}

HRESULT CEffect_Guard::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

    return S_OK;
}

HRESULT CEffect_Guard::NativeConstruct(const _uint _iSceneID, void* pArg)
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
	m_Desc.iNumInstance = 100;
	CVIBuffer_PointInstance_Explosion::PIDESC Desc;
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

	m_backupDesc = Desc;

	setActive(false);
	return S_OK;
}

_int CEffect_Guard::Tick(_double TimeDelta)
{
	m_pBuffer->Update(g_dDeltaTime, m_Desc.iAxis);

	m_fNonActiveTimeAcc += (_float)g_dDeltaTime;
	if (4.f <= m_fNonActiveTimeAcc)
	{
		setActive(false);
		m_fNonActiveTimeAcc = 0.f;
	}

	m_pTransform->Face_Target(g_pGameInstance->Get_CamPosition(L"Camera_Silvermane"));

	_matrix matCullingBoxPivot = XMMatrixIdentity();
	matCullingBoxPivot.r[3] = { m_Desc.CullingBoxPos.x, m_Desc.CullingBoxPos.y,m_Desc.CullingBoxPos.z, 1.f };
	m_pBox->Update_Matrix(matCullingBoxPivot * m_pTransform->Get_WorldMatrix());

    return 0;
}

_int CEffect_Guard::LateTick(_double TimeDelta)
{
	_bool bCulling = g_pGameInstance->isIn_WorldFrustum(m_pBox->Get_Points(), 20.f);
	if (true == bCulling)
	{
		if (nullptr != m_pRenderer)
		{
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_NONALPHA, this);
		}
	}

	return 0;
}

HRESULT CEffect_Guard::Render()
{
	m_pBox->Render(L"Camera_Silvermane");

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

	m_pBuffer->SetUp_ValueOnShader("g_fLifeTime", &m_Desc.fMaxLifeTime, sizeof(_float));
	m_pBuffer->SetUp_ValueOnShader("g_fCurTime", &m_Desc.fCurTime, sizeof(_float));

	_float3 color = { 0.3f, 0.3f, 1.f };
	m_pBuffer->SetUp_ValueOnShader("g_color", &color, sizeof(_float3));

	m_pBuffer->SetUp_ValueOnShader("g_vCamPosition", (void*)&CamPos, sizeof(_vector));

	m_pBuffer->Render(m_Desc.iRenderPassNum); //4번 

	return S_OK;
}

CEffect* CEffect_Guard::Copy()
{
	CEffect_Guard* pEffect = new CEffect_Guard(m_pDevice, m_pDeviceContext);
	if (FAILED(pEffect->NativeConstruct_Prototype()))
	{
		MSGBOX("Effect_Guard Copy Fail");
		Safe_Release(pEffect);
	}
	if (FAILED(pEffect->NativeConstruct(m_iSceneID, &m_Desc)))
	{
		MSGBOX("Effect_Guard Copy Fail");
		Safe_Release(pEffect);
	}
	
	return pEffect;
}

void CEffect_Guard::Set_Reset(_bool bReset)
{
	CEffect::Set_Reset(bReset);
	m_Desc.fCurTime = 0.f;
	m_pBuffer->Set_Desc(m_backupDesc);
	m_pBuffer->Particle_Reset();
}

HRESULT CEffect_Guard::SetUp_Components()
{
	if (!m_pTexture || !m_pRenderer || !m_pTransform)
		return E_FAIL;

	wstring tag = m_Desc.TextureTag;
	wstring NewTag = L"Texture_" + tag;
	if (FAILED(m_pTexture->Change_Texture(NewTag)))
		return E_FAIL;

	_vector vPos = { XMVectorGetX(m_Desc.fMyPos), XMVectorGetY(m_Desc.fMyPos), XMVectorGetZ(m_Desc.fMyPos), 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	//cullingbox Setting
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
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_PointInstance_Explosion", L"Com_VIBuffer", (CComponent**)&m_pBuffer, &m_backupDesc)))
		return E_FAIL;

	return S_OK;
}

CEffect_Guard* CEffect_Guard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CEffect_Guard* pInstance = new CEffect_Guard(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CEffect_Guard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Guard::Clone(const _uint _iSceneID, void* pArg)
{
	/* 복제본 생성할때는 아래함수 호출해서 추가 초기화를 진행 */
	CEffect_Guard* pInstance = new CEffect_Guard(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CEffect_Guard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Guard::Free()
{
	Safe_Release(m_pBox);
	Safe_Release(m_pBuffer);

	__super::Free();
}
