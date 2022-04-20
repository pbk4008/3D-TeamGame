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

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pAnimModel->SetUp_AnimationIndex(0);

	return S_OK;
}

_int CExplosion_Rock::Tick(_double _dDeltaTime)
{
	m_pAnimModel->Update_CombinedTransformationMatrix(_dDeltaTime * 5.f);

	_matrix matScale = XMMatrixScaling(0.0004f, 0.0004f, 0.0004f);
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

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Explosion_Rock_Up_Anim", L"Explosion_Rock_Anim", (CComponent**)&m_pAnimModel)))
		return E_FAIL;

	_matrix matScale = XMMatrixScaling(0.0003f, 0.0003f, 0.0003f);
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
	CGameObject::Free();
	Safe_Release(m_pAnimModel);
}
