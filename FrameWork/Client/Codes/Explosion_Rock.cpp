#include "pch.h"
#include "Explosion_Rock.h"
#include "Animation.h"

CExplosion_Rock::CExplosion_Rock()
{
}

CExplosion_Rock::CExplosion_Rock(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect(pDevice, pDeviceContext)
	, m_pAnimModel(nullptr)
{
}

CExplosion_Rock::CExplosion_Rock(const CExplosion_Rock& rhs)
	: CEffect(rhs)
	, m_pAnimModel(rhs.m_pAnimModel)
{
	Safe_AddRef(m_pAnimModel);
}

HRESULT CExplosion_Rock::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::EXPLOSION_ROCK;

	return S_OK;
}

HRESULT CExplosion_Rock::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(CGameObject::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (nullptr != _pArg)
	{
		memcpy(&m_Desc, _pArg, sizeof(ROCKINFO));
	}

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pAnimModel->SetUp_AnimationIndex(0);

	return S_OK;
}

_int CExplosion_Rock::Tick(_double _dDeltaTime)
{
	m_pAnimModel->Update_CombinedTransformationMatrix(_dDeltaTime * 5.f);
	_matrix matScale = XMMatrixIdentity();
	if ((_uint)SCENEID::SCENE_STAGE3 == g_pGameInstance->getCurrentLevel() || (_uint)SCENEID::SCENE_TEST_JS == g_pGameInstance->getCurrentLevel())
	{
		matScale = XMMatrixScaling(0.0007f, 0.0007f, 0.0007f);
	}
	else
	{
		matScale = XMMatrixScaling(0.0005f, 0.0005f, 0.0005f);
	}

	m_pAnimModel->Set_PivotMatrix(matScale);
	
	if (true == m_pAnimModel->Get_IsAnimFinished())
	{
		m_pAnimModel->SetUp_AnimationIndex(0);
		setActive(false);
	}
	
	return _int();
}

_int CExplosion_Rock::LateTick(_double _dDeltaTime)
{
	if (!m_pRenderer)
		return E_FAIL;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CExplosion_Rock::Render()
{
	_matrix smatWorld, smatView, smatProj;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pAnimModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pAnimModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pAnimModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
		return E_FAIL;
	for (_uint i = 0; i < m_pAnimModel->Get_NumMeshContainer(); ++i)
		m_pAnimModel->Render(i, 0);
	
	return S_OK;
}

HRESULT CExplosion_Rock::Render_Velocity()
{
	//wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	//SCB desc;
	//ZeroMemory(&desc, sizeof(SCB));

	//RIM rimdesc;
	//ZeroMemory(&rimdesc, sizeof(RIM));

	//MOTIONBLUR motion;
	//ZeroMemory(&motion, sizeof(MOTIONBLUR));
	//// velocity desc
	//_float4x4 rot;
	//XMStoreFloat4x4(&rot, m_pTransform->Get_WorldMatrix()
	//	* g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW)
	//	* g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	//rot._11 = 1.0f; rot._22 = 1.0f; rot._33 = 1.0f;
	//rot._41 = 0.0f; rot._42 = 0.0f; rot._43 = 0.0f;
	//motion.RotationMat = rot;
	//_matrix prewvp = g_pGameInstance->GetPreViewProtj(m_PreWroldMat);
	//XMStoreFloat4x4(&motion.preWorldViewPorjMat, prewvp);
	////----------------------------------------------------

	//CActor::BindConstantBuffer(wstrCamTag, &desc, &rimdesc, &motion);
	//for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	//{
	//	if (i != 2)
	//	{
	//		if (FAILED(m_pModel->Render(i, 5))) MSGBOX("Fialed To Rendering Silvermane");
	//	}
	//}


	//m_PreWroldMat = m_pTransform->Get_WorldMatrix();
	////m_timer += g_fDeltaTime;
	////if (m_timer >= 0.3f)
	////{
	////	m_PreWroldMat = m_pTransform->Get_WorldMatrix();
	////	m_timer = 0.f;
	////}


	return S_OK;
}

CEffect* CExplosion_Rock::Copy()
{
	CExplosion_Rock* pEffect = new CExplosion_Rock(m_pDevice, m_pDeviceContext);
	if (FAILED(pEffect->NativeConstruct_Prototype()))
	{
		MSGBOX("CExplosion_Rock Copy Fail");
		Safe_Release(pEffect);
	}
	if (FAILED(pEffect->NativeConstruct(m_iSceneID, &m_Desc)))
	{
		MSGBOX("CExplosion_Rock Copy Fail");
		Safe_Release(pEffect);
	}

	return pEffect;
}

void CExplosion_Rock::Set_Reset(_bool bReset)
{
	m_pAnimModel->SetUp_AnimationIndex(0);
}

HRESULT CExplosion_Rock::Ready_Component()
{
	CTransform::TRANSFORMDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));
	tDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tDesc.fSpeedPerSec = 0.f;

	m_pTransform->Set_TransformDesc(tDesc);

	wstring tag = m_Desc.ModelTag;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, tag, L"Explosion_Rock_Anim", (CComponent**)&m_pAnimModel)))
		return E_FAIL;

	_matrix matScale = XMMatrixScaling(0.0007f, 0.0007f, 0.0007f);
	m_pAnimModel->Set_PivotMatrix(matScale);
	
	return S_OK;
}

CExplosion_Rock* CExplosion_Rock::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CExplosion_Rock* pInstance = new CExplosion_Rock(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CExplosion_Rock Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CEffect* CExplosion_Rock::Clone(const _uint iSceneID, void* pArg)
{
	CExplosion_Rock* pInstance = new CExplosion_Rock(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CExplosion_Rock Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CExplosion_Rock::Free()
{
	CEffect::Free();
	Safe_Release(m_pAnimModel);
}
