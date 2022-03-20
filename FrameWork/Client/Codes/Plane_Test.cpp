#include "pch.h"
#include "Plane_Test.h"

CPlane_Test::CPlane_Test(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CPlane_Test::CPlane_Test(const CPlane_Test& _rhs)
	: CGameObject(_rhs)
	, m_vecNaviPoints(_rhs.m_vecNaviPoints)
{
}

HRESULT CPlane_Test::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	if(FAILED(Ready_NaviPoints(L"../Data/NavMesh/2222.dat")))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlane_Test::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_NaviMesh()))
		return E_FAIL;

	return S_OK;
}

_int CPlane_Test::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CPlane_Test::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

#ifdef _DEBUG
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
#endif
	return _int();
}

HRESULT CPlane_Test::Render()
{
	_fmatrix matWorld = m_pTransform->Get_WorldMatrix();
	auto vCamPos = g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");

	auto matWorldInvers = XMMatrixTranspose(matWorld);
	auto matViewInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	auto matProjInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &matWorldInvers, sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &matViewInvers, sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &matProjInvers, sizeof(_matrix));
	m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, 0);

	m_pVIBuffer->SetUp_ValueOnShader("g_vCamPosition", &vCamPos, sizeof(_vector));

	m_pVIBuffer->Render(0);

	return S_OK;
}

HRESULT CPlane_Test::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(m_iSceneID, L"VIBuffer_Plane", L"VIBuffer", (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* Com_Texture  */
	wstring TexTag = L"Plane_Texture";
	m_pTexture = (CTexture*)g_pGameInstance->Clone_Component(m_iSceneID, L"Plane_Texture", &TexTag);

	CMeshCollider::MESHDESC tMeshColliderDesc;
	tMeshColliderDesc.pParent = this;
	XMStoreFloat4x4(&tMeshColliderDesc.matTransform, XMMatrixIdentity());

	return S_OK;
}

HRESULT CPlane_Test::Ready_NaviMesh()
{
	CMeshCollider::MESHDESC NaviMeshDesc;
	CPhysicsXSystem::COLDESC ColDesc;

	ColDesc.eType = CPhysicsXSystem::ACTORTYPE::ACTOR_STATIC;
	ColDesc.fPos = { 0.f, 0.f, 0.f };
	ColDesc.bGravity = false;
	ColDesc.bKinematic = false;

	NaviMeshDesc.vecPoints.reserve(m_vecNaviPoints.size());
	for (_int i = 0; i < m_vecNaviPoints.size(); ++i)
		NaviMeshDesc.vecPoints.emplace_back(m_vecNaviPoints[i].vPoints);

	XMStoreFloat4x4(&NaviMeshDesc.matTransform, XMMatrixIdentity());
	NaviMeshDesc.pParent = this;
	NaviMeshDesc.tColDesc = ColDesc;

	m_pNaviCollider = (CMeshCollider*)g_pGameInstance->Clone_Component(m_iSceneID, L"Proto_Component_MeshCollider", &NaviMeshDesc);

	if (!m_pNaviCollider)
		return E_FAIL;

	/* Com_Texture  */
	//wstring TexTag = L"Plane_Texture";
	//m_pTexture = (CTexture*)g_pGameInstance->Clone_Component((_uint)SCENEID::SCENE_TEST_YM, L"Plane_Texture", &TexTag);

	return S_OK;
}

HRESULT CPlane_Test::Ready_NaviPoints(const wstring & _wstrFilePath)
{
	if(FAILED(g_pGameInstance->LoadFile<NAVIMESHDESC>(m_vecNaviPoints, _wstrFilePath.c_str())))
		return E_FAIL;
	m_vecNaviPoints.shrink_to_fit();

	return S_OK;
}

CPlane_Test* CPlane_Test::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CPlane_Test* pInstance = new CPlane_Test(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CPlane_Test Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlane_Test::Clone(const _uint _iSceneID, void* _pArg)
{
	CPlane_Test* pInstance = new CPlane_Test(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CPlane_Test Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlane_Test::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pNaviCollider);

	__super::Free();
}
