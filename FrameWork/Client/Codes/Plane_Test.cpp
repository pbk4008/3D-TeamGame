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

	m_iObectTag = (_uint)GAMEOBJECT::ENVIRONMENT;

	return S_OK;
}

HRESULT CPlane_Test::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	//if (FAILED(Ready_Components()))
	//	return E_FAIL;
	wstring wstrNaviPath = (*(wstring*)_pArg);
	if (FAILED(Ready_NaviPoints(wstrNaviPath)))
		return E_FAIL;

	if (FAILED(Ready_NavMesh()))
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
	//_fmatrix matWorld = m_pTransform->Get_WorldMatrix();
	//auto vCamPos = g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");

	//auto matWorldInvers = XMMatrixTranspose(matWorld);
	//auto matViewInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	//auto matProjInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	//m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &matWorldInvers, sizeof(_matrix));
	//m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &matViewInvers, sizeof(_matrix));
	//m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &matProjInvers, sizeof(_matrix));
	//m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, 0);

	//m_pVIBuffer->SetUp_ValueOnShader("g_vCamPosition", &vCamPos, sizeof(_vector));

	//m_pVIBuffer->Render(0);

	return S_OK;
}

HRESULT CPlane_Test::Ready_Components()
{
	/* Com_VIBuffer */
	//if (FAILED(__super::SetUp_Components(m_iSceneID, L"VIBuffer_Plane", L"VIBuffer", (CComponent**)&m_pVIBuffer)))
	//	return E_FAIL;

	/* Com_Texture  */
	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	m_pTexture->Change_Texture(L"Plane_Texture");

	return S_OK;
}

HRESULT CPlane_Test::Ready_NavMesh()
{
	CCollider::DESC ColDesc;
	ColDesc.eRigidType = ERigidType::Static;
	ColDesc.isSceneQuery = true;
	ColDesc.pGameObject = this;

	CNavMeshCollider::DESC NavMeshDesc;
	NavMeshDesc.tColliderDesc = ColDesc;
	NavMeshDesc.vecPoints.reserve(m_vecNaviPoints.size());
	for (_int i = 0; i < m_vecNaviPoints.size(); ++i)
		NavMeshDesc.vecPoints.emplace_back(m_vecNaviPoints[i].vPoints);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_NavMeshCollider", L"NavMeshCollider", (CComponent**)&m_pNaviCollider, &NavMeshDesc)))
		return E_FAIL;
	m_pNaviCollider->setShapeLayer((_uint)ELayer::Enviroment);

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
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pNaviCollider);

	__super::Free();
}
