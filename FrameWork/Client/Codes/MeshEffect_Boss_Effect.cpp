#include "pch.h"
#include "MeshEffect_Boss_Effect.h"

#include "Material.h"
//보스이펙트테스트로씀 
CMeshEffect_Boss_Effect::CMeshEffect_Boss_Effect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CMeshEffect(_pDevice, _pDeviceContext)
{
}

CMeshEffect_Boss_Effect::CMeshEffect_Boss_Effect(const CMeshEffect_Boss_Effect& _rhs)
	: CMeshEffect(_rhs)
{
}

HRESULT CMeshEffect_Boss_Effect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_pMaterial = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_MeshEffect", L"../bin/ShaderFile/Shader_MeshEffect.hlsl", CMaterial::EType::Static);

	return S_OK;
}

HRESULT CMeshEffect_Boss_Effect::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fAlpha = 1.f;
	m_fFlowSpeedAlpha = 1.f;

	m_vScale = { 20.f, 20.f, 20.f };
	m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });
	return S_OK;
}

_int CMeshEffect_Boss_Effect::Tick(_double _dDeltaTime)
{
	_int iProcess = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	m_fAccTime += (_float)_dDeltaTime;

	if (g_pGameInstance->getkeyDown(DIK_NUMPAD7))
	{
		//////////////////////// 태그
		//// 트레일
		// TrailBase
		// DistortionBase
		// DistortionMask
		// WispTrail
		// WispTrail_02
		// WispTrail_Thin
		// Fire_02
		// Wisp_01
		// EnergyPlasma_Tile_H_01
		// FluidTile01_DA
		// Lightning_Beam_Tile
		// Melee_Arc_02
		// PerlinElectric_Packed
		// Plasma_Seamless
		// SpeedTrails_Packed
		// Streak_Soft_Trail
		// Venus_Trail
		//// 메쉬
		// WispTrail_02_Red
		// Hexgrid
		// spk_17
		// magic_aura01
		// magic_electric00
		// magic_electric01
		// magic_electric02
		// wp01_007
		// myfire
		// waterfall02
		//// FX12
		// dist_noise
		// dust_longStar
		// flowmap
		// maskRGB3
		// maskRGB5
		// noise4
		// noise5
		// noise6
		// noise16
		//// FX13
		// dust01
		// dust02
		// fire01
		// flare_a
		// flare01
		// flareVertical02
		// ice02
		// lensFlare01
		// maskRGB06
		// smoke1
		// smoke03
		//// FXVerietyPack
		// decoLines
		// magicCircle020
		// noise17
		// T_ShieldPattern2
		if (FAILED(m_pMaterial->Change_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, L"T_ShieldPattern2")))
			return -1;

		if (FAILED(m_pMaterial->Change_Texture("g_MaskTexture", TEXTURETYPE::TEX_MASK, L"T_ShieldPattern2")))
			return -1;;
	}
	if (g_pGameInstance->getkeyDown(DIK_NUMPAD8))
	{
		///////////////////////// 태그
		// Model_MeshEffect
		// Model_Branch
		// Model_Magic_Electric00
		// Model_Magic_Electric01
		// Model_Magic_Electric02
		// Model_Outer
		// Model_MagicShield
		// Model_Parts
		// Model_Twirl
		// Model_Smoke
		// Model_Sphere00
		//// FX12
		// Model_Circle_Midpoly
		// Model_CylinderBend04
		// Model_Needle01
		// Model_Quad_CenterRot
		// Model_Sphere2
		// Model_SphereHarf03
		//// FX13
		// Model_AuraHousya
		// Model_Circle_Inv
		// Model_Circle_Inv2
		// Model_Circle_Lowpoly
		// Model_CircleDist5
		// Model_CircleDist06
		// Model_Cylinder_LowPoly_Idst
		// Model_Cylinder_SuperLow_Inv
		// Model_CylinderBend03
		// Model_CylinderBend03b
		// Model_CylinderBend06
		// Model_Lans
		// Model_LansHarf
		// Model_Quads
		// Model_RayOffset
		// Model_Tornado06
		// Model_Tornado06_Back
		// Model_WindLine30
		// Model_WindSmoke
		// Model_WindSmokeVertRot
		//// FXVerietyPack
		// Model_AuraWall_DistB_Kai
		// Model_Cylinder_LowPoly_Dist
		// Model_Cylinder_SuperLow
		// Model_CylinderBend07
		// Model_PlaneBend
		// Model_Tornado07
		Safe_Release(m_pModel);
		m_pModel = g_pGameInstance->Clone_Component<CModel>((_uint)SCENEID::SCENE_STATIC, L"Model_Sphere2");
		_uint iNumMesh = m_pModel->Get_NumMeshContainer();
		for (_uint i = 0; i < iNumMesh; ++i)
		{
			if (FAILED(m_pModel->Add_Material(m_pMaterial, i)))
				return -1;
		}
	}


	///////////////////////////////////// Alpha
	//if(0.f < m_fAlpha)
	//	m_fAlpha += -2.f * (_float)_dDeltaTime;
	//m_fAlpha += m_fFlowSpeedAlpha * (_float)_dDeltaTime;
	//if (0.f >= m_fAlpha)
	//	m_fFlowSpeedAlpha = 1.f;
	//else if (1.f <= m_fAlpha)
	//	m_fFlowSpeedAlpha = -1.f;
	m_fAlpha = 2.f;

	///////////////////////////////////// UV
	m_isReverse = false;
	m_vTiling.x = 15.f;
	m_vTiling.y = 15.f;
	//// X
	//m_isFlowX = true;
	//m_fFlowSpeedX = 0.2f;
	//m_vPlusUV.x += m_fFlowSpeedX * (_float)_dDeltaTime;
	//if (1.f < m_vPlusUV.x)
	//	m_vPlusUV.x = -1.f;
	//else if (-1.f > m_vPlusUV.x)
	//	m_vPlusUV.x = 1.f;
	//// Y
	//m_isFlowY = true;
	//m_fFlowSpeedY = 0.2f;
	//m_vPlusUV.y += m_fFlowSpeedY * (_float)_dDeltaTime;
	//if (1.f < m_vPlusUV.y)
	//	m_vPlusUV.y = -1.f;
	//else if (-1.f > m_vPlusUV.y)
	//	m_vPlusUV.y = 1.f;
	///////////////////////////////// Color
	m_isCustomColor = true;
	m_vColor = { 0.1f, 0.3f, 1.f };


	//m_vScale = { 20.f, 20.f, 20.f };
	//////////////////////////////////////////// Scale

	if (false == m_bBeSmall)
	{
		m_vScale.x += 200.f * (_float)_dDeltaTime;
		m_vScale.y += 200.f * (_float)_dDeltaTime;
		m_vScale.z += 200.f * (_float)_dDeltaTime;
	}

	if (30.f < m_vScale.x)
	{
		m_bBeSmall = true;
	}

	if (m_bBeSmall)
	{
		m_vScale.x -= 50.f * (_float)_dDeltaTime;
		m_vScale.y -= 50.f * (_float)_dDeltaTime;
		m_vScale.z -= 50.f * (_float)_dDeltaTime;
	}

	if (5.f >= m_vScale.x)
	{
		m_bBeSmall = false;
	}

	m_vScale = { 20.f, 20.f, 20.f };
	m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });

	//////////////////////////////////////////// Rotation
	m_pTransform->SetUp_Rotation(_float3(90.f, 0.f, 0.f));

	///////////////////////////////////////////// Position
	m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, -4.f, 0.f, 1.f });

	return _int();
}

_int CMeshEffect_Boss_Effect::LateTick(_double _dDeltaTime)
{
	_int iProcess = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	if (m_fAccTime > m_fLifeTime)
	{
		m_fAccTime = 0.f;
	}

	//if (0.05f < m_vScale.x)
	//{
	//	m_bRemove = true;
	//}

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CMeshEffect_Boss_Effect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_float Weight = 1.f;
	m_pModel->SetUp_ValueOnShader("g_Weight", &Weight, sizeof(_float));

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 6)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMeshEffect_Boss_Effect::Ready_Components()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Texture", L"MaskTexture", (CComponent**)&m_pMaskTexture)))
		return E_FAIL;

	m_pMaskTexture->Change_Texture(L"smoke03");
	if (FAILED(m_pMaterial->Set_Texture("g_MaskTexture", TEXTURETYPE::TEX_MASK, m_pMaskTexture, 0)))
		return E_FAIL;

	if (FAILED(m_pTexture->Change_Texture(L"smoke03")))
		return E_FAIL;
	if (FAILED(m_pMaterial->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, m_pTexture, 0)))
		return E_FAIL;
	Safe_AddRef(m_pTexture);

	m_pModel = g_pGameInstance->Clone_Component<CModel>((_uint)SCENEID::SCENE_STATIC, L"Model_AuraHousya");
	if (FAILED(m_pModel->Add_Material(m_pMaterial, 0)))
		return E_FAIL;

	return S_OK;
}

CMeshEffect_Boss_Effect* CMeshEffect_Boss_Effect::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMeshEffect_Boss_Effect* pInstance = new CMeshEffect_Boss_Effect(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMeshEffect_Boss_Effect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMeshEffect_Boss_Effect::Clone(const _uint _iSceneID, void* _pArg)
{
	CMeshEffect_Boss_Effect* pInstance = new CMeshEffect_Boss_Effect(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("Failed to Creating Clone CMeshEffect_Boss_Effect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshEffect_Boss_Effect::Free()
{
	__super::Free();
	Safe_Release(m_pMaskTexture);
}
