#include "Mouse.h"
#include "Transform.h"
#include "PipeLine.h"
#include "UI.h"

CMouse::CMouse()
{
	ZeroMemory(&m_vRayPos, sizeof(_float3));
	ZeroMemory(&m_vRayDir, sizeof(_float3));
	ZeroMemory(&m_tMousePos, sizeof(POINT));
}

CMouse::CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
	ZeroMemory(&m_vRayPos, sizeof(_float3));
	ZeroMemory(&m_vRayDir, sizeof(_float3));
	ZeroMemory(&m_tMousePos, sizeof(POINT));
}

HRESULT CMouse::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMouse::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CMouse::Tick(HWND hWnd, _double TimeDelta)
{
	GetCursorPos(&m_tMousePos);
	ScreenToClient(hWnd, &m_tMousePos);
	
	return _int(); 
}

void CMouse::RayUpdate(const wstring& pCamTag)
{

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

	CPipeLine* pInstance = GET_INSTANCE(CPipeLine);
	_matrix matProj, matView;

	matProj = pInstance->Get_Transform(pCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION);
	matProj = XMMatrixInverse(nullptr, matProj);

	matView = pInstance->Get_Transform(pCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	matView = XMMatrixInverse(nullptr, matView);

	RELEASE_INSTANCE(CPipeLine);

	vMouse = XMVector3TransformCoord(vMouse, matProj);

	_vector vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector vRayDir = vMouse - vRayPos;
	vRayDir = XMVector3Normalize(vRayDir);

	vRayPos = XMVector3TransformCoord(vRayPos, matView);
	vRayDir = XMVector3TransformNormal(vRayDir, matView);

	XMStoreFloat3(&m_vRayPos, vRayPos);
	XMStoreFloat3(&m_vRayDir, vRayDir);
}

CUI* CMouse::getCheckUI(list<CGameObject*>* pObjList)
{
	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
	_uint iIndex = 1;
	m_pDeviceContext->RSGetViewports(&iIndex, &viewPort);

	for (auto& pObj : *pObjList)
	{
		CTransform* pUITransform = pObj->Get_Component<CTransform>(L"Transform");

		if (!pUITransform)
			return false;

		_vector vPos = pUITransform->Get_State(CTransform::STATE_POSITION);

		_float fPosX = XMVectorGetX(vPos) + (viewPort.Width*0.5f);
		_float fPosY = (XMVectorGetY(vPos)*-1)+(viewPort.Height*0.5f);
		_float fSizX = pUITransform->Get_Scale(CTransform::STATE_RIGHT);
		_float fSizY = pUITransform->Get_Scale(CTransform::STATE_UP);

		RECT      rc;
		SetRect(&rc, (_uint)(fPosX - fSizX * 0.5f), (_uint)(fPosY - fSizY * 0.5f),
			(_uint)(fPosX + fSizX * 0.5f), (_uint)(fPosY + fSizY * 0.5f));

		_bool bPick = false;
		if (TRUE == PtInRect(&rc, m_tMousePos))
			bPick = true;
		else
			bPick = false;
		
		if (bPick)
			return static_cast<CUI*>(pObj);
	}
	return nullptr;
}


_fvector CMouse::Terrain_Picking(void* pVertices, _fmatrix matWorld, _uint iVtxX, _uint iVtxZ, _int& iHitIndex)
{
	_vector vRayPos = XMLoadFloat3(&m_vRayPos);
	vRayPos=XMVectorSetW(vRayPos, 1.f);
	_vector vRayDir = XMLoadFloat3(&m_vRayDir);

	_matrix matInverseWrold = XMMatrixInverse(nullptr, matWorld);

	vRayPos = XMVector3TransformCoord(vRayPos, matInverseWrold);
	vRayDir = XMVector3TransformNormal(vRayDir, matInverseWrold);


	//if (XMVectorGetX(vRayPos) < 0.f ||
	//	XMVectorGetZ(vRayPos) < 0.f)
	//	return XMVectorZero();

	VTXTEX* pTerrainsVertices = (VTXTEX*)pVertices;

	for (_uint i = 0; i < iVtxZ - 1; i++)
	{
		for (_uint j = 0; j < iVtxX - 1; j++)
		{
			_uint iIndex = i * iVtxX + j;

			_float fDist = 0.f;
			_vector TriPoint[3];

			TriPoint[0] = XMLoadFloat3(&pTerrainsVertices[iIndex + iVtxX].vPosition);
			TriPoint[1] = XMLoadFloat3(&pTerrainsVertices[iIndex + iVtxX+1].vPosition);
			TriPoint[2] = XMLoadFloat3(&pTerrainsVertices[iIndex + 1].vPosition);

 			if (TriangleTests::Intersects(vRayPos, vRayDir, TriPoint[0], TriPoint[1], TriPoint[2], fDist))
			{
				iHitIndex = iIndex;
				vRayPos += (fDist * vRayDir);
				return vRayPos;
			}

			TriPoint[0] = XMLoadFloat3(&pTerrainsVertices[iIndex + iVtxX].vPosition);
			TriPoint[1] = XMLoadFloat3(&pTerrainsVertices[iIndex + 1].vPosition);
			TriPoint[2] = XMLoadFloat3(&pTerrainsVertices[iIndex].vPosition);

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

CMouse* CMouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMouse* pInstance = new CMouse(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMouse Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMouse::Clone(void* pArg)
{
	CMouse* pInstance = new CMouse(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMouse Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}
