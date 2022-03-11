#include "pch.h"
#include "Plane.h"
#include "GameInstance.h"
#include "Mouse.h"
#include "NavSphere.h"
#include "GameObject.h"
#include "Observer.h"
#include "Cell.h"
#include "VIBuffer_Triangle.h"
#include "Navigation.h"

CPlane::CPlane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
: CGameObject(_pDevice, _pDeviceContext)
{
}

CPlane::CPlane(const CPlane& _rhs)
	:CGameObject(_rhs)
{
}

HRESULT CPlane::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlane::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

 	if (FAILED(SetUp_Components()))
		return E_FAIL;
	
	m_pTransform->Set_TransformDesc(10.0f, 45.0f);
	return S_OK;
}

_int CPlane::Tick(_double TimeDelta)
{
	Input_Key(TimeDelta);

	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
		Pick_Model();

	m_pObserver = GET_INSTANCE(CObserver);

	if (CObserver::MODE_NAV == m_pObserver->m_eMode)
	{
		if (3 == m_iPointindex)
			Make_NavigationCell();
		else if(true == m_bOneClick)
			Create_NavigationLine();
	}
	RELEASE_INSTANCE(CObserver);


	return _int();
}

_int CPlane::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);


	Update_CellPos();
	return _int();
}

HRESULT CPlane::Render()
{
	Set_WVPMatrix();

#ifdef _DEBUG
	if (nullptr != m_pObserver->m_pMeshCollider)
		m_pObserver->m_pMeshCollider->Render(L"Camera");

	if(m_bRender)
		m_pVIBufferCom->Render(0);
	m_pNavigationCom->Render(L"Camera");
#endif // _DEBUG

	return S_OK;
}

HRESULT CPlane::SetUp_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TAB_STATIC, L"Prototype_Component_VIBuffer_Plane", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	/* Com_Navigation */
	if (FAILED(__super::SetUp_Components(TAB_STATIC, L"Prototype_Component_Navigation", L"Com_Navigation", (CComponent**)&m_pNavigationCom)))
		return E_FAIL;
	/* Com_Texture  */
	wstring TexTag = L"Plane_Texture";
	m_pTexture = (CTexture*)g_pGameInstance->Clone_Component(TAB_STATIC, L"Texture", &TexTag);

	return S_OK;
}

HRESULT CPlane::Create_NavigationLine()
{
	m_bOneClick = false;

	if (true == m_pObserver->m_bNavSpherePick)
	{
		CNavSphere* pSphere = dynamic_cast<CNavSphere*>(m_pObserver->m_pNavSphere);
		m_fPoints[m_iPointindex++] = &(pSphere->m_fPostion);
		m_pObserver->m_bNavSpherePick = false;
		return S_OK;
	}
	else
	{
		CGameObject* pSphere = nullptr;
		if (SUCCEEDED(g_pGameInstance->Add_GameObjectToLayer(TAB_STATIC, L"Layer_NaveSphere", L"Prototype_GameObject_NavSphere", &XMVector3TransformCoord(XMLoadFloat3(&m_fLocalMouse), m_pTransform->Get_WorldMatrix())  /*&m_pObserver->m_fPickPos*/, &pSphere)))
		{
			m_fPoints[m_iPointindex++] = &(dynamic_cast<CNavSphere*>(pSphere)->m_fPostion);
			return S_OK;
		}
		else
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CPlane::Update_CellPos()
{
	CNavSphere* pSphere = m_pObserver->m_pNavSphere;
	if (nullptr == pSphere)
		return E_FAIL;

	if (FAILED(m_pNavigationCom->Update_Buffer(XMLoadFloat3(&pSphere->m_fPostion))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlane::Make_NavigationCell(CCell* _pCell/*= nullptr*/)
{
	CCell* pCell = nullptr;

	if (nullptr == _pCell)
	{
		CCW_Sort((*m_fPoints));
		pCell = CCell::Create(m_pDevice, m_pDeviceContext, m_fPoints, m_pNavigationCom->m_Cells.size());
	}
	else
	{
		memcpy(m_fPoints, _pCell->m_vPoint, sizeof(_float3) * CCell::POINT_END);
		CCW_Sort((*m_fPoints));
		pCell = CCell::Create(m_pDevice, m_pDeviceContext, *m_fPoints, m_pNavigationCom->m_Cells.size());
	}

	m_pNavigationCom->m_Cells.push_back(pCell);
	m_iPointindex = 0;

	cout << "Cell Create Done" << endl;

	return S_OK;
}

void CPlane::CCW_Sort(_float3* pPoints)
{
	_int temp = (pPoints[0].x * pPoints[1].z) +
		(pPoints[1].x * pPoints[2].z) +
		(pPoints[2].x * pPoints[0].z);

	temp = temp - (pPoints[0].z * pPoints[1].x) -
		(pPoints[1].z * pPoints[2].x) -
		(pPoints[2].z * pPoints[0].x);

	if (temp > 0)
	{
		_float3 fPointTemp = pPoints[1];
		pPoints[1] = pPoints[2];
		pPoints[2] = fPointTemp;
		CCW_Sort(pPoints);
	}
	else if (temp < 0)
		return;
}

void CPlane::Input_Key(_double _dtimeDelta)
{
	if (g_pGameInstance->getkeyPress(DIK_NUMPAD8))
		m_pTransform->Go_Straight(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_NUMPAD2))
		m_pTransform->Go_BackWard(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_NUMPAD4))
		m_pTransform->Go_Left(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_NUMPAD6))
		m_pTransform->Go_Right(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_NUMPAD7))
		m_pTransform->Mesh_Up(_dtimeDelta);
	if (g_pGameInstance->getkeyPress(DIK_NUMPAD9))
		m_pTransform->Mesh_Down(_dtimeDelta);

	if (g_pGameInstance->getkeyPress(DIK_NUMPAD1))
		m_bRender = TRUE;
	if (g_pGameInstance->getkeyPress(DIK_NUMPAD3))
		m_bRender = FALSE;
}

void CPlane::Pick_Model(void)
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

	FACEINDICES32* Indices = m_pVIBufferCom->Get_Indices();
	void* pVertices = m_pVIBufferCom->Get_Vertices();
	_uint iNumFaces = m_pVIBufferCom->Get_NumFaces();
	_float fBary1, fBary2;
	_float fDist;

	m_bOneClick = false;

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_float3 v0 = ((VTXTEX*)pVertices)[Indices[i]._0].vPosition;
		_float3 v1 = ((VTXTEX*)pVertices)[Indices[i]._1].vPosition;
		_float3 v2 = ((VTXTEX*)pVertices)[Indices[i]._2].vPosition;

		// Check if the pick ray passes through this point
		if (IntersectTriangle(vRayPos, vRayDir, v0, v1, v2, &fDist, &fBary1, &fBary2))
		{
			XMStoreFloat3(&m_fLocalMouse, vRayPos + (vRayDir * fDist));
			pObserver->m_fPickPos = m_fLocalMouse;
			pObserver->m_bPlanePick = true;
			pObserver->m_pPlane = this;
			m_bOneClick = true;
		}
	}

	RELEASE_INSTANCE(CObserver);
}

void CPlane::Find_Pointer(void)
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

_bool CPlane::IntersectTriangle(const _fvector& orig, const _fvector& dir, _float3& v0, _float3& v1, _float3& v2, _float* t, _float* u, _float* v)
{
	// Find vectors for two edges sharing vert0
	_vector edge1 = XMLoadFloat3(&v1) - XMLoadFloat3(&v0);

	_vector edge2 = XMLoadFloat3(&v2) - XMLoadFloat3(&v0);

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
		tvec = orig - XMLoadFloat3(&v0);
	}
	else
	{
		tvec = XMLoadFloat3(&v0) - orig;
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

void CPlane::Set_WVPMatrix()
{
	_fmatrix matWorld = m_pTransform->Get_WorldMatrix();
	auto vCamPos = g_pGameInstance->Get_CamPosition(L"Camera");

	auto matWorldInvers = XMMatrixTranspose(matWorld);
	auto matViewInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	auto matProjInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &matWorldInvers, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &matViewInvers, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &matProjInvers, sizeof(_matrix));
	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, 0);

	m_pVIBufferCom->SetUp_ValueOnShader("g_vCamPosition", &vCamPos, sizeof(_vector));
}

CPlane* CPlane::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CPlane* pInstance = new CPlane(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CPlane!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlane::Clone(void* pArg)
{
	CPlane* pInstance = new CPlane(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CPlane!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
} 

void CPlane::Free(void)
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pVIBufferCom);
}
