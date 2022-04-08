#include "pch.h"
#include "Treasure_Chest.h"

CTreasure_Chest::CTreasure_Chest(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CTreasure_Chest::CTreasure_Chest(const CTreasure_Chest& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CTreasure_Chest::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreasure_Chest::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	m_vecWorldMat = (*(MABOBJECT*)_pArg).WorldMat;
	m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&m_vecWorldMat));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

_int CTreasure_Chest::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CTreasure_Chest::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return -1;
	return _int();
}

HRESULT CTreasure_Chest::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_CombinedMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_float4 color = _float4(0.25, 0.92, 0.94, 1.f);
	_float power = 1.5f;

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_color", &color, sizeof(_float4))))		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_empower", &power, sizeof(_float))))		return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 0)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CTreasure_Chest::Ready_Components()
{
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 0.f;
	tTransformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Treasure_Chest", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	return S_OK;
}

void CTreasure_Chest::Raycast_FromMouse(const _double& _dDeltaTime)
{
	_matrix smatView;

	smatView = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	smatView = XMMatrixInverse(nullptr, smatView);

	_vector svRayPos, svRayDir;
	memcpy_s(&svRayPos, sizeof(_vector), &smatView.r[3], sizeof(_vector));
	memcpy_s(&svRayDir, sizeof(_vector), &smatView.r[2], sizeof(_vector));
	svRayDir = XMVector3Normalize(svRayDir);

	_float fOutDist = 0.f;
	if (m_pCollider->Raycast_AABB(svRayPos, svRayDir, fOutDist))
	{
		m_fHoldTime += (_float)_dDeltaTime;
		m_isPick = true;
	}
	else
	{
		m_fHoldTime = 0.f;
		m_isPick = false;
	}
}

_bool CTreasure_Chest::Raycast(const _fvector& _svRayPos, const _fvector& _svRayDir, _float& _fOutDist, const _double & _dDeltaTime)
{
	if(m_pCollider->Raycast_AABB(_svRayPos, _svRayDir, _fOutDist))
	{
		m_fHoldTime += (_float)_dDeltaTime;
		m_isPick = true;
	}
	else
	{
		m_fHoldTime = 0.f;
		m_isPick = false;
	}

	return m_isPick;
}

CTreasure_Chest* CTreasure_Chest::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CTreasure_Chest* pInstance = new CTreasure_Chest(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTreasure_Chest Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTreasure_Chest::Clone(const _uint _iSceneID, void* _pArg)
{
	CTreasure_Chest* pInstance = new CTreasure_Chest(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CTreasure_Chest Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTreasure_Chest::Free()
{
	Safe_Release(m_pCollider);
	Safe_Release(m_pAnimationController);
	Safe_Release(m_pModel);

	__super::Free();
}
