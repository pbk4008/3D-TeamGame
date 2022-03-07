#include "pch.h"
#include "ToolMouse.h"
#include "ToolObject.h"

CToolMouse::CToolMouse()
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
{
	ZeroMemory(&m_vMousePos, sizeof(_float2));
	ZeroMemory(&m_vRayPos, sizeof(_float2));
	ZeroMemory(&m_vRayDir, sizeof(_float2));
}

CToolMouse::CToolMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	ZeroMemory(&m_vMousePos, sizeof(_float2));
	ZeroMemory(&m_vMousePos, sizeof(_float3));
}

HRESULT CToolMouse::Init_Mouse()
{
	return S_OK;
}

_uint CToolMouse::Update(_float fDeltaTime)
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_ToolHwnd, &pt);

	m_vMousePos.x = (_float)pt.x;
	m_vMousePos.y= (_float)pt.y;
	
	return 0;
}

_fvector CToolMouse::UI_Picking()
{
	if (m_vMousePos.x < 0
		|| m_vMousePos.x>g_iToolCnX
		|| m_vMousePos.y<0
		|| m_vMousePos.y>g_iToolCnY)
		return XMVectorZero();

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->GetMouseState(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		D3D11_VIEWPORT vp;
		ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
		_uint pNumViewPorts = 1;
		m_pDeviceContext->RSGetViewports(&pNumViewPorts, &vp);

		_float fPointX = (m_vMousePos.x / (0.5f * vp.Width)) - 1.f;
		_float fPointY = (m_vMousePos.y / (-0.5f * vp.Height)) + 1.f;

		_vector vMouse = XMVectorSet
		(
			fPointX,
			fPointY,
			0.f,
			1.f
		);

		_matrix matProj, matView;

		matProj = pInstance->GetTransform(L"MainCamera", TRANSMATRIXSTATE::D3DT_PROJ);
		matProj = XMMatrixInverse(nullptr, matProj);

		vMouse = XMVector3TransformCoord(vMouse, matProj);

		matView = pInstance->GetTransform(L"MainCamera", TRANSMATRIXSTATE::D3DT_VIEW);
		matView = XMMatrixInverse(nullptr, matView);

		vMouse = XMVector3TransformCoord(vMouse, matView);
		
		return vMouse;
	}
	RELEASE_INSTANCE(CGameInstance);

	return XMVectorZero();
}

_fvector CToolMouse::Terrain_Picking(void* pVertices, _fmatrix matWorld, _uint iVtxX, _uint iVtxZ, _int& iHitIndex)
{
	if (m_vMousePos.x < 0
		|| m_vMousePos.x>g_iToolCnX
		|| m_vMousePos.y<0
		|| m_vMousePos.y>g_iToolCnY)
		return XMVectorZero();

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	_uint pNumViewPorts = 1;
	m_pDeviceContext->RSGetViewports(&pNumViewPorts, &vp);

	_float fPointX = (m_vMousePos.x / (0.5f * vp.Width)) - 1.f;
	_float fPointY = (m_vMousePos.y / (-0.5f * vp.Height)) + 1.f;

	_vector vMouse = XMVectorSet
	(
		fPointX,
		fPointY,
		0.f,
		1.f
	);

	_matrix matProj, matView;

	matProj = pInstance->GetTransform(L"MainCamera", TRANSMATRIXSTATE::D3DT_PROJ);
	matProj = XMMatrixInverse(nullptr, matProj);

	vMouse = XMVector3TransformCoord(vMouse, matProj);

	matView = pInstance->GetTransform(L"MainCamera", TRANSMATRIXSTATE::D3DT_VIEW);
	matView = XMMatrixInverse(nullptr, matView);

	_vector vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector vRayDir = vMouse - vRayPos;
	vRayDir = XMVector3Normalize(vRayDir);

	vRayPos = XMVector3TransformCoord(vRayPos, matView);
	vRayDir = XMVector3TransformNormal(vRayDir, matView);

	_matrix matInverseWorld = XMMatrixInverse(nullptr, matWorld);

	vRayPos = XMVector3TransformCoord(vRayPos, matInverseWorld);
	vRayDir = XMVector3TransformNormal(vRayDir, matInverseWorld);

	XMStoreFloat3(&m_vRayPos, vRayPos);
	XMStoreFloat3(&m_vRayDir, vRayDir);


	RELEASE_INSTANCE(CGameInstance);

	VTXTERRAIN* pTerrainVertices = ((VTXTERRAIN*)pVertices);

	for (_uint i = 0; i < iVtxZ - 1; i++)
	{
		for (_uint j = 0; j < iVtxX - 1; j++)
		{
			_uint iIndex = i * iVtxX + j;

			_float fDist = 0.f;
			_vector TriPoint[3];

			//À§ »ï°¢Çü
			TriPoint[0] = XMLoadFloat3(&pTerrainVertices[iIndex + iVtxX].vPosition);
			TriPoint[1] = XMLoadFloat3(&pTerrainVertices[iIndex + iVtxX + 1].vPosition);
			TriPoint[2] = XMLoadFloat3(&pTerrainVertices[iIndex + 1].vPosition);

			if (TriangleTests::Intersects(vRayPos, vRayDir, TriPoint[0], TriPoint[1], TriPoint[2], fDist))
			{
				iHitIndex = iIndex;
				vRayPos += (fDist * vRayDir);
				return vRayPos;
			}

			//¾Æ·¡ »ï°¢Çü
			TriPoint[0] = XMLoadFloat3(&pTerrainVertices[iIndex + iVtxX].vPosition);
			TriPoint[1] = XMLoadFloat3(&pTerrainVertices[iIndex + 1].vPosition);
			TriPoint[2] = XMLoadFloat3(&pTerrainVertices[iIndex].vPosition);

			if (TriangleTests::Intersects(vRayPos, vRayDir, TriPoint[0], TriPoint[1], TriPoint[2], fDist))
			{
				iHitIndex = iIndex;
				vRayPos += (fDist * vRayDir);
				return vRayPos;
			}
		}
	}
	return XMVectorZero();
}

_int CToolMouse::Click_Object(vector<CToolObject*>* vecObjects)
{
	if (m_vMousePos.x < 0
		|| m_vMousePos.x>g_iToolCnX
		|| m_vMousePos.y<0
		|| m_vMousePos.y>g_iToolCnY)
		return 9999;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	_uint pNumViewPorts = 1;
	m_pDeviceContext->RSGetViewports(&pNumViewPorts, &vp);

	_float fPointX = (m_vMousePos.x / (0.5f * vp.Width)) - 1.f;
	_float fPointY = (m_vMousePos.y / (-0.5f * vp.Height)) + 1.f;

	_vector vMouse = XMVectorSet
	(
		fPointX,
		fPointY,
		0.f,
		1.f
	);

	_matrix matProj, matView;

	matProj = pInstance->GetTransform(L"MainCamera", TRANSMATRIXSTATE::D3DT_PROJ);
	matProj = XMMatrixInverse(nullptr, matProj);

	vMouse = XMVector3TransformCoord(vMouse, matProj);

	matView = pInstance->GetTransform(L"MainCamera", TRANSMATRIXSTATE::D3DT_VIEW);
	matView = XMMatrixInverse(nullptr, matView);

	_vector vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector vRayDir = vMouse - vRayPos;
	vRayDir = XMVector3Normalize(vRayDir);

	vRayPos = XMVector3TransformCoord(vRayPos, matView);
	vRayDir = XMVector3TransformNormal(vRayDir, matView);

	XMStoreFloat3(&m_vRayPos, vRayPos);
	XMStoreFloat3(&m_vRayDir, vRayDir);

	RELEASE_INSTANCE(CGameInstance);
	_uint iSize = (_uint)vecObjects->size();

	for (auto& pObj : *vecObjects)
		pObj->ResetColliderColor();

	for (_uint i = 0; i < iSize; i++)
	{
		CCollider* pCollider = (*vecObjects)[i]->getClickCollider();
		_bool bCollision = false;
		switch (pCollider->getType())
		{
		case CCollider::COLLIDER_TYPE::TYPE_AABB:
			bCollision = pCollider->CollisionRay_AABB(vRayPos, vRayDir);
			break;
		case CCollider::COLLIDER_TYPE::TYPE_OBB:
			break;
		case CCollider::COLLIDER_TYPE::TYPE_SPHERE:
			break;
		}
		if (bCollision)
			return i;
	}
	return -1;
}

CToolMouse* CToolMouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CToolMouse* pInstance = new CToolMouse(pDevice,pDeviceContext);
	if (FAILED(pInstance->Init_Mouse()))
	{
		MSG_BOX(L"CToolMouse Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CToolMouse::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
