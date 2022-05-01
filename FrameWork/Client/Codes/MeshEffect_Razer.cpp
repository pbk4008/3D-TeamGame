#include "pch.h"
#include "MeshEffect_Razer.h"

#include "Material.h"

CMeshEffect_Razer::CMeshEffect_Razer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMeshEffect(_pDevice, _pDeviceContext)
{
}

CMeshEffect_Razer::CMeshEffect_Razer(const CMeshEffect_Razer& _rhs)
	: CMeshEffect(_rhs)
{
}

HRESULT CMeshEffect_Razer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_pMaterial = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_MeshEffect", L"../bin/ShaderFile/Shader_MeshEffect.hlsl", CMaterial::EType::Static);

	return S_OK;
}

HRESULT CMeshEffect_Razer::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fFlowSpeedAlpha = 1.f;
	m_vScale = { 1.f, 1.f, 1000.f };
	m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });

	///////////////////////////////////// Alpha
	m_fAlpha = 1.f;

	///////////////////////////////////// UV
	m_vTiling.x = 1.f;
	m_vTiling.y = 6.f;

	//////////////////////////////////////////// Rotation
	m_pTransform->SetUp_Rotation(_float3(90.f, 0.f, 0.f));

	return S_OK;
}

_int CMeshEffect_Razer::Tick(_double _dDeltaTime)
{
	_int iProcess = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	///////////////////////////////// Color
	m_isCustomColor = true;
	m_vColor = { 1.f, 0.5f, 0.3f };

	//////////////////////////////////////////// Scale
	m_vScale.x += (_float)_dDeltaTime * -1.f;
	if (0.f >= m_vScale.x)
		m_vScale.x = 0.f;
	m_vScale.y += (_float)_dDeltaTime * -1.f;
	if (0.f >= m_vScale.y)
		m_vScale.y = 0.f;
	m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });

	return _int();
}

_int CMeshEffect_Razer::LateTick(_double _dDeltaTime)
{
	_int iProcess = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	if (m_fAccTime > m_fLifeTime)
	{
		m_fAccTime = 0.f;
	}

	if (0.1f >= m_vScale.x)
	{
		OVERLAPDESC tOverlapDesc;
		tOverlapDesc.geometry = PxSphereGeometry(2.f);
		XMStoreFloat3(&tOverlapDesc.vOrigin, m_pTransform->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 0.f, 0.f, 0.f });
		CGameObject* pHitObject = nullptr;
		tOverlapDesc.ppOutHitObject = &pHitObject;
		tOverlapDesc.filterData.flags = PxQueryFlag::eANY_HIT | PxQueryFlag::eDYNAMIC;
		tOverlapDesc.layerMask = (1 << (_uint)ELayer::Player);
		if (g_pGameInstance->Overlap(tOverlapDesc))
		{
			if (pHitObject)
			{
				ATTACKDESC tAttackDesc;
				tAttackDesc.fDamage = 15.f;
				tAttackDesc.iLevel = 4;
				tAttackDesc.pOwner = this;
				tAttackDesc.pHitObject = this;
				static_cast<CActor*>(pHitObject)->Hit(tAttackDesc);
			}
		}

		//ÀÌÆåÆ®¿Â
		CEffect* pEffect = CEffectManager::GetInstance()->Get_Effect((_uint)EFFECT::EXPLOSION_ROCK_UP);
		pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 0.f, 0.f, 0.f });
		pEffect->Set_Reset(true);
		pEffect->setActive(true);

		pEffect = CEffectManager::GetInstance()->Get_Effect((_uint)EFFECT::ATTACK_GROUND);
		pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 0.f, 0.f, 0.f });
		pEffect->Set_Reset(true);
		pEffect->setActive(true);

		pEffect = CEffectManager::GetInstance()->Get_Effect((_uint)EFFECT::HIT_GROUND); //·¹ÀÌÀúÀÌÆåÆ®·Î ¹Ù²ãÁà¾ßµÉµí 
		pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 0.f, 0.f, 0.f });
		pEffect->Set_Reset(true);
		pEffect->setActive(true);

		pEffect = CEffectManager::GetInstance()->Get_Effect((_uint)EFFECT::BOSS_RAZER_SMOKE); //·¹ÀÌÀúÀÌÆåÆ®·Î ¹Ù²ãÁà¾ßµÉµí 
		pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 0.f, 0.f, 0.f });
		pEffect->Set_Reset(true);
		pEffect->setActive(true);

		//½¦ÀÌÅ·
		CCameraShake::SHAKEEVENT tShakeEvent;
		tShakeEvent.fDuration = 1.f;
		tShakeEvent.fBlendInTime = 0.3f;
		tShakeEvent.fBlendOutTime = 0.7f;
		tShakeEvent.tWaveX.fAmplitude = -0.15f;
		tShakeEvent.tWaveX.fFrequency = 30.f;
		tShakeEvent.tWaveY.fAmplitude = 0.15f;
		tShakeEvent.tWaveY.fFrequency = 20.f;
		tShakeEvent.tWaveZ.fAdditionalOffset = 0.2f;
		tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
		tShakeEvent.fInnerRadius = 10.f;
		tShakeEvent.fOuterRadius = 20.f;
		tShakeEvent.fDistanceRate = 10.f;

		g_pShakeManager->Shake(tShakeEvent, m_pTransform->Get_State(CTransform::STATE_POSITION));

		//ÃÊ±âÈ­
		m_vScale = { 1.f, 1.f, 1000.f };
		m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });
		setActive(false);
	}

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CMeshEffect_Razer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_float weight = 0.3f;
	m_pModel->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float));

	_float empower = 0.35f;
	m_pModel->SetUp_ValueOnShader("g_empower", &empower, sizeof(_float));


	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 9)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMeshEffect_Razer::Ready_Components()
{
	if (FAILED(m_pTexture->Change_Texture(L"Venus_Trail")))
		return E_FAIL;

	if (FAILED(m_pMaterial->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, m_pTexture, 0)))
		return E_FAIL;
	Safe_AddRef(m_pTexture);

	m_pModel = g_pGameInstance->Clone_Component<CModel>((_uint)SCENEID::SCENE_STATIC, L"Model_Cylinder_LowPoly_Idst");
	if (FAILED(m_pModel->Add_Material(m_pMaterial, 0)))
		return E_FAIL;

	return S_OK;
}

CMeshEffect_Razer* CMeshEffect_Razer::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMeshEffect_Razer* pInstance = new CMeshEffect_Razer(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMeshEffect_Razer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMeshEffect_Razer::Clone(const _uint _iSceneID, void* _pArg)
{
	CMeshEffect_Razer* pInstance = new CMeshEffect_Razer(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("Failed to Creating Clone CMeshEffect_Razer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshEffect_Razer::Free()
{
	__super::Free();
}
