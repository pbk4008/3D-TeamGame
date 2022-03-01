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

	m_ModelDesc = *(MODELDESC*)pArg;
	
	if(m_ModelDesc.WorldMat._42 > -100.f)
		m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&m_ModelDesc.WorldMat));

	XMStoreFloat4x4(&m_matWorld,m_pTransform->Get_WorldMatrix());
	m_matWorld._41 = m_ModelDesc.fInitPos.x;
	m_matWorld._42 = m_ModelDesc.fInitPos.y;
	m_matWorld._43 = m_ModelDesc.fInitPos.z;

	m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&m_matWorld));
	m_pTransform->SetTransformDesc(10.0f, 45.0f);

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	
	m_MeshDesc.eLevel = m_ModelDesc.eLevel;
	_tcscpy_s(m_MeshDesc.FolderName, m_ModelDesc.strFolder.c_str());
	_tcscpy_s(m_MeshDesc.FileName, m_ModelDesc.strFileName.c_str());
	_tcscpy_s(m_MeshDesc.Name, m_ModelDesc.strName.c_str());
	_tcscpy_s(m_MeshDesc.Tag, m_ModelDesc.strTag.c_str());

	XMStoreFloat3(&m_MeshDesc.fInitPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
	XMStoreFloat4x4(&m_MeshDesc.WorldMat, m_pTransform->Get_WorldMatrix());
	 
	return S_OK;
}

_int CStatic_Mesh::Tick(_double TimeDelta)
{
	return _int();
}

_int CStatic_Mesh::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	if(TKEY_DOWN(VK_RBUTTON))
		Pick_Model();

	if (true == m_bPick)
	{
		CObserver* pObserver = GET_INSTANCE(CObserver);

		Input_Key(TimeDelta);

		XMStoreFloat3(&pObserver->m_fModelPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
		pObserver->m_fModelScale.x = XMVectorGetX(m_pTransform->Get_State(CTransform::STATE_RIGHT));
		pObserver->m_fModelScale.y = XMVectorGetY(m_pTransform->Get_State(CTransform::STATE_UP));
		pObserver->m_fModelScale.z = XMVectorGetZ(m_pTransform->Get_State(CTransform::STATE_LOOK));

		XMStoreFloat3(&m_MeshDesc.fInitPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
		XMStoreFloat4x4(&m_MeshDesc.WorldMat, m_pTransform->Get_WorldMatrix());

		RELEASE_INSTANCE(CObserver);
	}

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
	if (FAILED(__super::SetUp_Components(TAB_MAP, m_ModelDesc.strFileName, L"Com_Model", (CComponent**)&m_pModelCom)))
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

_fmatrix CStatic_Mesh::Get_WorldMat(void)
{
	return m_pTransform->Get_WorldMatrix();
}

void CStatic_Mesh::Input_Key(_double _dtimeDelta)
{
	if(g_pGameInstance->getkeyPress(DIK_NUMPAD8))
		m_pTransform->Go_Straight(_dtimeDelta);	
	if (g_pGameInstance->getkeyPress(DIK_NUMPAD2))
		m_pTransform->Go_BackWard(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_NUMPAD4))
		m_pTransform->Go_Left(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_NUMPAD6))
		m_pTransform->Go_Right(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_PGUP))
		m_pTransform->Go_Up(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_PGDN))
		m_pTransform->Go_Down(_dtimeDelta);

	_long	MouseMove = 0;
	_long   MouseWheel = 0;
	if (g_pGameInstance->getkeyPress(DIK_Z))
	{
		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X))
		{
			m_pTransform->Rotation_Axis(XMVectorSet(1.f, 0.f, 0.f, 0.f), _dtimeDelta * MouseMove * 0.01f);
		}
	}
	if (g_pGameInstance->getkeyPress(DIK_X))
	{
		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X))
		{
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), _dtimeDelta * MouseMove * 0.01f);
		}
	}
	if (g_pGameInstance->getkeyPress(DIK_C))
	{
		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X))
		{
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 0.f, 1.f, 0.f), _dtimeDelta * MouseMove * 0.01f);
		}
	}

	if (g_pGameInstance->getkeyPress(DIK_C))
	{
		if (MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X))
		{
			m_pTransform->Rotation_Axis(XMVectorSet(0.f, 0.f, 1.f, 0.f), _dtimeDelta * MouseMove * 0.01f);
		}
	}

	if (MouseWheel = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_WHILL))
	{
		if (0 < MouseWheel)
		{
			_fvector Scale_UP = { 1.05f, 1.05f, 1.05f };
			m_pTransform->Scale_Up(Scale_UP);
		}
		else
		{
			_fvector Scale_Down = { 0.95f, 0.95f, 0.95f };
			m_pTransform->Scale_Up(Scale_Down);
		}
	}
}

void CStatic_Mesh::Pick_Model(void)
{
	CObserver* pObserver = GET_INSTANCE(CObserver);

	Find_Pointer();

	_vector vRayPos = XMLoadFloat3(&m_vRayPos);
	vRayPos = XMVectorSetW(vRayPos, 1.f);
	_vector vRayDir = XMLoadFloat3(&m_vRayDir);
	vRayDir = XMVector3Normalize(vRayDir);
	_matrix matInverseWrold = XMMatrixInverse(nullptr, m_pTransform->Get_WorldMatrix());

	vRayPos = XMVector3TransformCoord(vRayPos, matInverseWrold);
	vRayDir = XMVector3TransformNormal(vRayDir, matInverseWrold);

	/* Get the picked triangle */
	CMeshContainer* pMesh = m_pModelCom->Get_NumIndices(0);
	/* 모델따로 Terrain 따로 받아와야함 */
	FACEINDICES32* Indices = m_pModelCom->Get_Indices();

	void* pVertices = m_pModelCom->Get_Vertices();
	_uint iNumFaces = pMesh->Get_NumFaces();
	_float fBary1, fBary2;
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
		if (IntersectTriangle(vRayPos, vRayDir, Pos_1, Pos_2, Pos_3, &fDist, &fBary1, &fBary2))
		{
			m_bPick = true;
			pObserver->m_bPick = true;
			pObserver->m_pModel = this;
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

_bool CStatic_Mesh::IntersectTriangle(const _fvector& orig, const _fvector& dir, _fvector& v0, _fvector& v1, _fvector& v2, _float* t, _float* u, _float* v)
{
	// Find vectors for two edges sharing vert0
	_vector edge1 = v1 - v0;

	_vector edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	_vector pvec;
	pvec = XMVector3Cross(dir, edge2);

	// If determinant is near zero, ray lies in plane of triangle
	_vector temp = XMVector3Dot(edge1, pvec);
	_float det;
	XMStoreFloat(&det, temp);

	_vector tvec;
	if (det > 0)
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
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
	XMStoreFloat(v, XMVector3Dot(dir, qvec));
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
