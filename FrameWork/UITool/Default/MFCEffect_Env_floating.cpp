#include "pch.h"
#include "MFCEffect_Env_Floating.h"
#include "GameInstance.h"
#include "VIBuffer_PointInstance_Env_Floating.h"
#include "MainFrm.h"
#include "MyFormView.h"
#include "EffectTool_Dlg.h"


CMFCEffect_Env_Floating::CMFCEffect_Env_Floating()
{
}

CMFCEffect_Env_Floating::CMFCEffect_Env_Floating(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    :CEffect(pDevice,pDeviceContext)
{
}

CMFCEffect_Env_Floating::CMFCEffect_Env_Floating(const CEffect& rhs)
    :CEffect(rhs)
{
}

HRESULT CMFCEffect_Env_Floating::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

    return S_OK;
}

HRESULT CMFCEffect_Env_Floating::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
	{
		return E_FAIL;
	}

	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(EFFECTDESC));
		int a = 0;
	}

	//여기서 필요한 모든 컴포넌트들 Clone해옴
	if (FAILED(SetUp_Components())) 
	{
		return E_FAIL;
	}

	CVIBuffer_PointInstance_Env_Floating::PIDESC Desc;
	_tcscpy_s(Desc.ShaderFilePath, m_Desc.ShaderFilePath);
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

	m_pBuffer->Set_Desc(Desc);
	m_pBuffer->Particle_Reset();

	m_bReset = false;

	return S_OK;
}

_int CMFCEffect_Env_Floating::Tick(_double TimeDelta)
{
	_matrix mat, mat1;
	mat1 = XMMatrixIdentity();
	mat1.r[3] = { m_Desc.CullingBoxPos.x, m_Desc.CullingBoxPos.y,m_Desc.CullingBoxPos.z, 1.f };

	mat = m_pTransform->Get_WorldMatrix();
	m_pBox->Update_Matrix(mat1 * mat);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyFormView* pForm = dynamic_cast<CMyFormView*>(pMain->m_SplitterWnd.GetPane(0, 0));
	CEffectTool_Dlg* Dlg = dynamic_cast<CEffectTool_Dlg*>(&pForm->m_EffectDlg);
	if (nullptr != Dlg)
	{
		if (Dlg->m_bReset)
		{
			m_bReset = true;
			Dlg->m_bReset = false;
			memcpy(&m_Desc, &Dlg->m_EffectDesc, sizeof(EFFECTDESC));
		}
		else
		{
			m_bReset = false;
		}
	}

	if (!m_bReset)
	{
		m_pBuffer->Update(TimeDelta, m_Desc.iAxis);
	}

	if (m_bReset)
	{
		CVIBuffer_PointInstance_Env_Floating::PIDESC Desc;
		_tcscpy_s(Desc.ShaderFilePath, m_Desc.ShaderFilePath);
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
		m_pBox->Set_Length(m_Desc.CullingBoxSize.x, m_Desc.CullingBoxSize.y, m_Desc.CullingBoxSize.z);

		m_pBuffer->Set_Desc(Desc);
		m_pBuffer->Particle_Reset();

		m_bReset = false;
	}

	//z정렬
	_vector vDir = g_pGameInstance->Get_CamPosition(L"MFCCamera_Proj") - m_pTransform->Get_State(CTransform::STATE_POSITION);
	vDir = XMVector3Normalize(vDir);
	m_pBuffer->Set_Dir(vDir);

	_uint iAllFrameCount = (m_Desc.iImageCountX * m_Desc.iImageCountY);
	m_Desc.fFrame += (_float)(iAllFrameCount * TimeDelta * m_Desc.fEffectPlaySpeed); //플레이속도 
	if (m_Desc.fFrame >= iAllFrameCount)
	{
		m_Desc.fFrame = 0;
	}

	if (m_Desc.fMaxLifeTime > m_Desc.fCurTime)
	{
		m_Desc.fCurTime += TimeDelta;
	}

    return 0;
}

_int CMFCEffect_Env_Floating::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}

	return 0;
}

HRESULT CMFCEffect_Env_Floating::Render()
{
	m_pBox->Render(L"MFCCamera_Proj");

	//_matrix XMWorldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix));
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MFCCamera_Proj", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MFCCamera_Proj", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector CamPos = g_pGameInstance->Get_CamPosition(L"MFCCamera_Proj");

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

	m_pBuffer->SetUp_ValueOnShader("g_vCamPosition", (void*)&CamPos, sizeof(_vector));

	m_pBuffer->Render(m_Desc.iRenderPassNum);

	return S_OK;
}

HRESULT CMFCEffect_Env_Floating::SetUp_Components()
{
	if (!m_pTexture || !m_pRenderer || !m_pTransform)
		return E_FAIL;

	//이미지 리스트박스로부터 가져옴
	wstring tag = m_Desc.TextureTag;
	if (FAILED(m_pTexture->Change_Texture(tag)))
		return E_FAIL;

	_vector vPos = { XMVectorGetX(m_Desc.fMyPos), XMVectorGetY(m_Desc.fMyPos), XMVectorGetY(m_Desc.fMyPos), 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);


	//culling 
	m_pBox = g_pGameInstance->Clone_Component<CCullingBox>(0, L"Proto_Component_CullingBox");
	if (!m_pBox)
		return E_FAIL;
	m_pBox->Set_Length(m_Desc.CullingBoxSize.x, m_Desc.CullingBoxSize.y, m_Desc.CullingBoxSize.z);


	//버퍼 Clone
	CVIBuffer_PointInstance_Env_Floating::PIDESC Desc;
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

	if (FAILED(__super::SetUp_Components(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Prototype_Component_VIBuffer_PointInstance_Floating", L"Com_VIBuffer", (CComponent**)&m_pBuffer, &Desc)))
		return E_FAIL;

	return S_OK;
}

CEffect* CMFCEffect_Env_Floating::Copy()
{
	return nullptr;
}

CMFCEffect_Env_Floating* CMFCEffect_Env_Floating::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CMFCEffect_Env_Floating* pInstance = new CMFCEffect_Env_Floating(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMFCEffect_Env_Floating");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMFCEffect_Env_Floating::Clone(const _uint iSceneID, void* pArg)
{
	/* 복제본 생성할때는 아래함수 호출해서 추가 초기화를 진행 */
	CMFCEffect_Env_Floating* pInstance = new CMFCEffect_Env_Floating(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID ,pArg)))
	{
		MSGBOX("Failed to Creating Clone CMFCEffect_Env_Floating");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMFCEffect_Env_Floating::Free()
{
	__super::Free();
}
