#include "pch.h"
#include "MFCEffect.h"
#include "GameInstance.h"
#include "VIBuffer_PointInstance_Explosion.h"
#include "MainFrm.h"
#include "MyFormView.h"
#include "EffectTool_Dlg.h"


CMFCEffect::CMFCEffect()
{
}

CMFCEffect::CMFCEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    :CEffect(pDevice,pDeviceContext)
{
}

CMFCEffect::CMFCEffect(const CEffect& rhs)
    :CEffect(rhs)
{
}

HRESULT CMFCEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

    return S_OK;
}

HRESULT CMFCEffect::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
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

	CVIBuffer_PointInstance_Explosion::PIDESC Desc;
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

	m_pBuffer->Set_Desc(Desc);
	m_pBuffer->Particle_Reset();

	m_bReset = false;

	return S_OK;
}

_int CMFCEffect::Tick(_double TimeDelta)
{
	//m_pBuffer->Update(TimeDelta, m_Desc.iAxis);

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
		CVIBuffer_PointInstance_Explosion::PIDESC Desc;
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

_int CMFCEffect::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHA, this);
	}

	return 0;
}

HRESULT CMFCEffect::Render()
{
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

HRESULT CMFCEffect::SetUp_Components()
{
	if (!m_pTexture || !m_pRenderer || !m_pTransform)
		return E_FAIL;

	//이미지 리스트박스로부터 가져옴
	wstring tag = m_Desc.TextureTag;
	if (FAILED(m_pTexture->Change_Texture(tag)))
		return E_FAIL;

	_vector vPos = { XMVectorGetX(m_Desc.fMyPos), XMVectorGetY(m_Desc.fMyPos), XMVectorGetY(m_Desc.fMyPos), 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	//버퍼 Clone
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
	if (FAILED(__super::SetUp_Components(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Prototype_Component_VIBuffer_PointInstance_Explosion", L"Com_VIBuffer", (CComponent**)&m_pBuffer, &Desc)))
		return E_FAIL;

	return S_OK;
}

CMFCEffect* CMFCEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CMFCEffect* pInstance = new CMFCEffect(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMFCEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMFCEffect::Clone(void* pArg)
{
	/* 복제본 생성할때는 아래함수 호출해서 추가 초기화를 진행 */
	CMFCEffect* pInstance = new CMFCEffect(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Clone CMFCEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMFCEffect::Free()
{
	__super::Free();
}
