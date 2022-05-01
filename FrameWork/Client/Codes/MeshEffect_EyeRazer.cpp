#include "pch.h"
#include "MeshEffect_EyeRazer.h"

#include "Material.h"
#include "HierarchyNode.h"

CMeshEffect_EyeRazer::CMeshEffect_EyeRazer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMeshEffect(_pDevice, _pDeviceContext)
{
}

CMeshEffect_EyeRazer::CMeshEffect_EyeRazer(const CMeshEffect_EyeRazer& _rhs)
	: CMeshEffect(_rhs)
{
}

HRESULT CMeshEffect_EyeRazer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_pMaterial = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_MeshEffect", L"../bin/ShaderFile/Shader_MeshEffect.hlsl", CMaterial::EType::Static);

	return S_OK;
}

HRESULT CMeshEffect_EyeRazer::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	m_pTransform->Set_WorldMatrix(XMMatrixIdentity());

	m_fFlowSpeedAlpha = 1.f;

	m_vScale = { 1.f, 1.f, 1000.f };
	m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });

	///////////////////////////////////// Alpha
	m_fAlpha = 1.f;

	///////////////////////////////////// UV
	m_vTiling.x = 1.f;
	m_vTiling.y = 6.f;

	m_isCustomColor = true;
	m_vColor = { 1.f, 0.5f, 0.3f };

	return S_OK;
}

_int CMeshEffect_EyeRazer::Tick(_double _dDeltaTime)
{
	_int iProcess = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	if (nullptr != m_pEyeNode)
	{
		//눈에서나와서 플레이어방향으로세팅 
		CTransform* pBossTransform = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_Boss")->front()->Get_Transform();
		CModel* pBossModel = (CModel*)g_pGameInstance->getObjectList(m_iSceneID, L"Layer_Boss")->front()->Get_Component(L"Model");
		_matrix EyeMat = m_pEyeNode->Get_CombinedMatrix();

		EyeMat = XMMatrixRotationX(XMConvertToRadians(-90.f)) * EyeMat * pBossModel->Get_PivotMatrix() * pBossTransform->Get_WorldMatrix();

		m_pTransform->Set_WorldMatrix(EyeMat);
	}

	//플레이어를 향한 방향벡터
	_vector vBossPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector  vDist = vBossPos - g_pObserver->Get_PlayerPos();
	_vector vBossToPlayerLook = XMVector3Normalize(vDist);

	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	_matrix mat = m_pTransform->Get_WorldMatrix();

	m_fAngle = fDistToPlayer * (-0.4f * fDistToPlayer);

	if (-47.f >= m_fAngle)
	{
		m_fAngle = -47.f;
	}

	mat = XMMatrixRotationY(XMConvertToRadians(m_fAngle)) * mat;

	m_pTransform->Set_WorldMatrix(mat);
	
	//cout << m_fAngle << endl;

	return _int();
}

_int CMeshEffect_EyeRazer::LateTick(_double _dDeltaTime)
{
	_int iProcess = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	_vector vBossPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector  vDist = vBossPos - g_pObserver->Get_PlayerPos();
	_vector vBossToPlayerLook = XMVector3Normalize(vDist);

	_float fDistToPlayer = XMVectorGetX(XMVector3Length(vDist));

	if (false == m_bRotate)
	{
		m_bRotate = true;

		//플레이어를 향한 방향벡터
		m_vScale = { 0.5f, 0.5f, fDistToPlayer * 3.5f };

		//쉐이킹
		CCameraShake::SHAKEEVENT tShakeEvent;
		tShakeEvent.fDuration = 1.f;
		tShakeEvent.fBlendInTime = 0.3f;
		tShakeEvent.fBlendOutTime = 0.7f;
		tShakeEvent.tWaveX.fAmplitude = -0.1f;
		tShakeEvent.tWaveX.fFrequency = 20.f;
		tShakeEvent.tWaveY.fAmplitude = 0.1f;
		tShakeEvent.tWaveY.fFrequency = 10.f;
		tShakeEvent.tWaveZ.fAdditionalOffset = 0.2f;
		tShakeEvent.tWaveZ.fAdditionalOffset = -1.f;
		tShakeEvent.fInnerRadius = 10.f;
		tShakeEvent.fOuterRadius = 20.f;
		tShakeEvent.fDistanceRate = 10.f;

		g_pShakeManager->Shake(tShakeEvent, g_pObserver->Get_PlayerPos());
	}

	m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });


	_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector svLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_vector svRight = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT));
	
	//Razer
	CEffect* pEffect = CEffectManager::GetInstance()->Get_Effect((_uint)EFFECT::RAZER);
	pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION,
		m_pTransform->Get_State(CTransform::STATE_POSITION) + (m_pTransform->Get_Scale(CTransform::STATE_LOOK) * svLook * -0.3f));

	//RazerSmoke
	CEffect* pEffect2 = CEffectManager::GetInstance()->Get_Effect((_uint)EFFECT::BOSS_RAZER_SMOKE);
	pEffect2->Get_Transform()->Set_State(CTransform::STATE_POSITION,
		m_pTransform->Get_State(CTransform::STATE_POSITION) + (m_pTransform->Get_Scale(CTransform::STATE_LOOK) * svLook * -0.3f));


	if (false == m_bEffectOn && 0.f <= m_fEffectAcc)
	{
		//이펙트온

		pEffect->Set_Reset(true);
		pEffect->setActive(true);

		pEffect2->Set_Reset(true);
		pEffect2->setActive(true);

		m_bEffectOn = true;
	}

	if (m_bEffectOn)
	{
		m_fEffectAcc += (_float)_dDeltaTime;
	}

	if (0.05f <= m_fEffectAcc)
	{
		m_bEffectOn = false;
		m_fEffectAcc = 0.f;
	}

	//충돌
	SWEEPDESC tSweepDesc;
	tSweepDesc.geometry = PxSphereGeometry(2.5f);
	XMStoreFloat3(&tSweepDesc.vOrigin, m_pTransform->Get_State(CTransform::STATE_POSITION));
	XMStoreFloat3(&tSweepDesc.vDir, -1.f * XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)));
	tSweepDesc.fMaxDistance = m_pTransform->Get_Scale(CTransform::STATE_LOOK);
	tSweepDesc.filterData.flags = PxQueryFlag::eANY_HIT | PxQueryFlag::eDYNAMIC;
	tSweepDesc.layerMask = (1 << (_uint)ELayer::Player);
	CGameObject* pHitObject = nullptr;
	tSweepDesc.ppOutHitObject = &pHitObject;

	if (g_pGameInstance->Sweep(tSweepDesc))
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

	if (0.1f >= m_vScale.x)
	{
		//이펙트온
		//CEffect* pEffect = CEffectManager::GetInstance()->Get_Effect((_uint)EFFECT::EXPLOSION_ROCK_UP);
		//pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 0.f, 0.f, 0.f });
		//pEffect->Set_Reset(true);
		//pEffect->setActive(true);
		
		
		//초기화
		m_vScale = { 1.f, 1.f, 1000.f };
		m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });
		setActive(false);
	}


	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CMeshEffect_EyeRazer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_float weight = 0.3f;
	m_pModel->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float));

	_float empower = 0.85f;
	m_pModel->SetUp_ValueOnShader("g_empower", &empower, sizeof(_float));


	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 9)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMeshEffect_EyeRazer::Ready_Components()
{
	if (FAILED(m_pTexture->Change_Texture(L"Venus_Trail")))
		return E_FAIL;

	if (FAILED(m_pMaterial->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, m_pTexture, 0)))
		return E_FAIL;
	Safe_AddRef(m_pTexture);

	m_pModel = g_pGameInstance->Clone_Component<CModel>((_uint)SCENEID::SCENE_STATIC, L"Model_Cylinder_LowPoly_Idst_Razer");
	if (FAILED(m_pModel->Add_Material(m_pMaterial, 0)))
		return E_FAIL;

	return S_OK;
}

CMeshEffect_EyeRazer* CMeshEffect_EyeRazer::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMeshEffect_EyeRazer* pInstance = new CMeshEffect_EyeRazer(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMeshEffect_EyeRazer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMeshEffect_EyeRazer::Clone(const _uint _iSceneID, void* _pArg)
{
	CMeshEffect_EyeRazer* pInstance = new CMeshEffect_EyeRazer(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("Failed to Creating Clone CMeshEffect_EyeRazer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshEffect_EyeRazer::Free()
{
	__super::Free();
}
