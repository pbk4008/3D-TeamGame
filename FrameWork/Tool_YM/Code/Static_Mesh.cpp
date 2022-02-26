#include "pch.h"
#include "Static_Mesh.h"
#include "GameInstance.h"
#include "MeshContainer.h"
#include "Observer.h"

CStatic_Mesh::CStatic_Mesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CStatic_Mesh::CStatic_Mesh(const CStatic_Mesh& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CStatic_Mesh::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatic_Mesh::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	m_ModelDesc = (MODELDESC*)pArg;
	XMStoreFloat4x4(&m_matWorld, m_pTransform->Get_WorldMatrix());

	m_matWorld._41 = m_ModelDesc->fInitPos.x;
	m_matWorld._42 = m_ModelDesc->fInitPos.y;
	m_matWorld._43 = m_ModelDesc->fInitPos.z;
	
	m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&m_matWorld));
	m_pTransform->SetTransformDesc(10.0f, 45.0f);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CStatic_Mesh::Tick(_double TimeDelta)
{
	return _int();
}

_int CStatic_Mesh::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	//if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_RBUTTON))
	if(TKEY_DOWN(VK_RBUTTON))
		Pick_Model();

	if(true == m_bPick)
		Input_Key(TimeDelta);

	return _int();
}

HRESULT CStatic_Mesh::Render()
{
	Set_WVPMatrix();

	if (nullptr != m_pModelCom)
	{
		if (FAILED(m_pModelCom->Bind_Buffers()))
			return E_FAIL;

		for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
		{
			m_pModelCom->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE);
			if(false == m_bPick)
				m_pModelCom->Render(i, 0);
			else
				m_pModelCom->Render(i, 1);
		}
	}

	return S_OK;
}

HRESULT CStatic_Mesh::SetUp_Components()
{
	/* Com_Model */
	if (FAILED(__super::SetUp_Components(TAB_MAP, m_ModelDesc->strFileName, L"Com_Model", (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

void CStatic_Mesh::Set_WVPMatrix(void)
{
	_fmatrix matWorld = m_pTransform->Get_WorldMatrix();
	auto vCamPos = g_pGameInstance->Get_CamPosition(L"Camera");
	 
	auto matWorldInvers = XMMatrixTranspose(matWorld);
	auto matViewInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	auto matProjInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &matWorldInvers, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &matViewInvers, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &matProjInvers, sizeof(_matrix));
}

CGameObject* CStatic_Mesh::Clone(void* pArg)
{
	CStatic_Mesh* pInstance = new CStatic_Mesh(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CStatic_Mesh!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CStatic_Mesh::Input_Key(_double _dtimeDelta)
{
	if(g_pGameInstance->getkeyPress(DIK_UP))
		m_pTransform->Go_Straight(_dtimeDelta);	
	if (g_pGameInstance->getkeyPress(DIK_DOWN))
		m_pTransform->Go_BackWard(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_LEFT))
		m_pTransform->Go_Left(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_RIGHT))
		m_pTransform->Go_Right(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_PGUP))
		m_pTransform->Go_Up(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_PGDN))
		m_pTransform->Go_Down(_dtimeDelta);
}

void CStatic_Mesh::Pick_Model(void)
{
	CObserver* pObserver = GET_INSTANCE(CObserver);

	Find_Pointer();

	_vector vRayPos = XMLoadFloat3(&m_vRayPos);
	vRayPos = XMVectorSetW(vRayPos, 1.f);
	_vector vRayDir = XMLoadFloat3(&m_vRayDir);

	_matrix matInverseWrold = XMMatrixInverse(nullptr, m_pTransform->Get_WorldMatrix());

	vRayPos = XMVector3TransformCoord(vRayPos, matInverseWrold);
	vRayDir = XMVector3TransformNormal(vRayDir, matInverseWrold);

	/* Get the picked triangle */
	CMeshContainer* pMesh = m_pModelCom->Get_NumIndices(0);
	/* 모델따로 Terrain 따로 받아와야함 */
	FACEINDICES32* Indices = m_pModelCom->Get_Indices();

	void* pVertices = m_pModelCom->Get_Vertices();
	_uint iNumFaces = pMesh->Get_NumFaces();
	_float fDist = 0.0f;
	m_bPick = false;
	pObserver->m_bPick = false;
	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_float3 v0 = ((VTXMESH*)pVertices)[Indices[i]._0].vPosition;
		_float3 v1 = ((VTXMESH*)pVertices)[Indices[i]._1].vPosition;
		_float3 v2 = ((VTXMESH*)pVertices)[Indices[i]._2].vPosition;

		_vector Pos_1 = XMLoadFloat3(&v0);
		_vector Pos_2 = XMLoadFloat3(&v1);
		_vector Pos_3 = XMLoadFloat3(&v2);

		// Check if the pick ray passes through this point
		if (TriangleTests::Intersects(vRayPos, vRayDir, Pos_1, Pos_2, Pos_3, fDist))
		{
			m_bPick = true;
			pObserver->m_bPick = true;
			pObserver->m_pModel = this;
			XMStoreFloat3(&pObserver->m_fModelPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			XMStoreFloat3(&m_fLocalMouse, XMLoadFloat3(&m_vRayPos) + (XMLoadFloat3(&m_vRayDir)) * fDist);
			cout << "Model Pick Pos :" << m_fLocalMouse.x << "//" << m_fLocalMouse.y << "//" << m_fLocalMouse.z << endl;
		}
	}

	RELEASE_INSTANCE(CObserver);
}

void CStatic_Mesh::Find_Pointer(void)
{
	GetCursorPos(&m_tMousePos);
	ScreenToClient(g_hWnd, &m_tMousePos);

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	_uint pNumViewPorts = 1;
	m_pDeviceContext->RSGetViewports(&pNumViewPorts, &vp);

	_float fPointX = (m_tMousePos.x / (0.5f * vp.Width)) - 1.f;
	_float fPointY = (m_tMousePos.y / (-0.5f * vp.Height)) + 1.f;
	_vector vMouse = XMVectorSet
	(
		fPointX,
		fPointY,
		0.f,
		1.f
	);

	_matrix matProj, matView;

	matProj = g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION);
	matProj = XMMatrixInverse(nullptr, matProj);

	matView = g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	matView = XMMatrixInverse(nullptr, matView);

	vMouse = XMVector3TransformCoord(vMouse, matProj);

	_vector vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector vRayDir = vMouse - vRayPos;
	vRayDir = XMVector3Normalize(vRayDir);

	vRayPos = XMVector3TransformCoord(vRayPos, matView);
	vRayDir = XMVector3TransformNormal(vRayDir, matView);

	XMStoreFloat3(&m_vRayPos, vRayPos);
	XMStoreFloat3(&m_vRayDir, vRayDir);
}

_bool CStatic_Mesh::IntersectTriangle(const _float3& orig, const _float3& dir, _float3& v0, _float3& v1, _float3& v2, _float* t, _float* u, _float* v)
{
	// Find vectors for two edges sharing vert0
	_vector edge1 = XMLoadFloat3(&v1) - XMLoadFloat3(&v0);

	_vector edge2 = XMLoadFloat3(&v2) - XMLoadFloat3(&v0);

	// Begin calculating determinant - also used to calculate U parameter
	_vector pvec;
	pvec = XMVector3Cross(XMLoadFloat3(&dir), edge2);

	// If determinant is near zero, ray lies in plane of triangle
	_vector temp = XMVector3Dot(edge1, pvec);
	_float det;
	XMStoreFloat(&det, temp);

	_vector tvec;
	if (det > 0)
	{
		tvec = XMLoadFloat3(&orig) - XMLoadFloat3(&v0);
	}
	else
	{
		tvec = XMLoadFloat3(&v0) - XMLoadFloat3(&orig);
		det = -det;
	}

	if (det < 0.0001f)
		return FALSE;

	// Calculate U parameter and test bounds
	XMStoreFloat(u, XMVector3Dot(tvec, pvec));

	if (*u < 0.0f || *u > det)
		return FALSE;

	// Prepare to test V parameter
	_vector qvec;
	qvec = XMVector3Cross(tvec, edge1);

	// Calculate V parameter and test bounds
	XMStoreFloat(v, XMVector3Dot(XMLoadFloat3(&dir), qvec));
	if (*v < 0.0f || *u + *v > det)
		return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	XMStoreFloat(t, XMVector3Dot(edge2, qvec));
	_float fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return TRUE;
}

CStatic_Mesh* CStatic_Mesh::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CStatic_Mesh* pInstance = new CStatic_Mesh(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CStatic_Mesh!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatic_Mesh::Free(void)
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
