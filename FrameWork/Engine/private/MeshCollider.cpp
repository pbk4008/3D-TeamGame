#include "MeshCollider.h"
#include "Gizmo.h"
CMeshCollider::CMeshCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CCollider(pDevice, pDeviceContext)
{
}

CMeshCollider::CMeshCollider(const CMeshCollider& rhs)
	: CCollider(rhs)
{
}

HRESULT CMeshCollider::NativeConstruct_Prototype()
{
	if (FAILED(CCollider::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshCollider::NativeConstruct(void* pArg)
{
	MESHDESC tDesc = (*(MESHDESC*)pArg);

	if (FAILED(CCollider::NativeConstruct(&tDesc.tColDesc.eType)))
		return E_FAIL;

	if (FAILED(Init_Shape(tDesc.vecPoints)))
		return E_FAIL;

	tDesc.tColDesc.fPos = _float3(tDesc.matTransform._41, tDesc.matTransform._42, tDesc.matTransform._43);
	if (FAILED(Init_Collider(tDesc.tColDesc)))
		return E_FAIL;

	if (!m_pRigidBody)
		return E_FAIL;

	m_pRigidBody->userData = tDesc.pParent;

	m_matLoaclMatrix = tDesc.matTransform;
	return S_OK;
}

HRESULT CMeshCollider::Render(const wstring& pCameraTag)
{
	if (m_pGizmo)
		return E_FAIL;

	PxTriangleMeshGeometry pxTriangleMesh;
	m_pShape->getTriangleMeshGeometry(pxTriangleMesh);
	_uint iTriangleCnt = pxTriangleMesh.triangleMesh->getNbTriangles();

	for (_uint i = 0; i < iTriangleCnt; i++)
	{
		_float3 pPoints[]
			= {
			_float3(pxTriangleMesh.triangleMesh->getVertices()[i * 3].x,
			pxTriangleMesh.triangleMesh->getVertices()[i * 3].y,
			pxTriangleMesh.triangleMesh->getVertices()[i * 3].z),

			_float3(pxTriangleMesh.triangleMesh->getVertices()[i * 3 + 1].x,
			pxTriangleMesh.triangleMesh->getVertices()[i * 3 + 1].y,
			pxTriangleMesh.triangleMesh->getVertices()[i * 3 + 1].z),

			_float3(pxTriangleMesh.triangleMesh->getVertices()[i * 3 + 2].x,
			pxTriangleMesh.triangleMesh->getVertices()[i * 3 + 2].y,
			pxTriangleMesh.triangleMesh->getVertices()[i * 3 + 2].z),
		};

		m_pGizmo->DrawMesh(pPoints, pCameraTag, XMLoadFloat4(&m_vColor));
	}

	return S_OK;
}

HRESULT CMeshCollider::Init_Shape(const vector<_float3*>& vecPoints)
{
	FACEINDICES32* Indices = new FACEINDICES32[(_uint)vecPoints.size()];
	for (_uint i = 0; i < (_uint)vecPoints.size(); i++)
	{
		Indices[i]._0 = i * 3;
		Indices[i]._1 = i * 3 + 1;
		Indices[i]._2 = i * 3 + 2;
	}

	PxTriangleMeshDesc meshDesc;

	meshDesc.points.data = &vecPoints;
	meshDesc.points.count = (_uint)vecPoints.size()*3;
	meshDesc.points.stride = sizeof(_float3);
	meshDesc.triangles.count = (_uint)vecPoints.size();
	meshDesc.triangles.data = Indices;
	meshDesc.triangles.stride = sizeof(FACEINDICES32);

	CPhysicsXSystem* pInstance = GET_INSTANCE(CPhysicsXSystem);


	m_pShape = pInstance->Init_Mesh(meshDesc);
	if (!m_pShape)
		return E_FAIL;

	RELEASE_INSTANCE(CPhysicsXSystem);

	return S_OK;
}

CMeshCollider* CMeshCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMeshCollider* pInstance = new CMeshCollider(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMesh Collider Crate Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CMeshCollider::Clone(void* pArg)
{
	CMeshCollider* pInstance = new CMeshCollider(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMesh Collider Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshCollider::Free()
{
	CCollider::Free();
}
