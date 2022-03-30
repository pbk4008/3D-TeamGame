#include "CullingBox.h"
#include "Gizmo.h"
#include "GameInstance.h"

CCullingBox::CCullingBox()
	: m_pGizmo(nullptr)
{
	ZeroMemory(m_pPoints, sizeof(_float4) * 8);
}

CCullingBox::CCullingBox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
	, m_pGizmo(nullptr)
{
	ZeroMemory(m_pPoints, sizeof(_float4) * 8);
}

CCullingBox::CCullingBox(const CCullingBox& rhs)
	: CComponent(rhs)
	, m_pGizmo(rhs.m_pGizmo)
{
	Safe_AddRef(m_pGizmo);
	for (_uint i = 0; i < 8; i++)
		m_pPoints[i] = rhs.m_pPoints[i];
}

HRESULT CCullingBox::NativeConstruct_Prototype()
{
	if (FAILED(CComponent::NativeConstruct_Prototype()))
		return E_FAIL;

	m_pGizmo = g_pGameInstance->Clone_Component<CGizmo>(0, L"Proto_Component_Gizmo");

	return S_OK;
}

HRESULT CCullingBox::NativeConstruct(void* pArg)
{
	if (FAILED(CComponent::NativeConstruct(pArg)))
		return E_FAIL;

	m_pPoints[0] = { -1.f, -1.f, -1.f, 1.f };
	m_pPoints[1] = {  1.f, -1.f, -1.f, 1.f };
	m_pPoints[2] = {  1.f, -1.f,  1.f, 1.f };
	m_pPoints[3] = { -1.f, -1.f,  1.f, 1.f };
	m_pPoints[4] = { -1.f,  1.f, -1.f, 1.f };
	m_pPoints[5] = { 1.f,  1.f, -1.f, 1.f };
	m_pPoints[6] = { 1.f,  1.f,  1.f, 1.f };
	m_pPoints[7] = { -1.f,  1.f,  1.f, 1.f };

	return S_OK;
}

HRESULT CCullingBox::Render(const wstring& pCameraTag)
{
	if (!m_pGizmo)
		return E_FAIL;

	_vector vPoints[8];
	for (_uint i = 0; i < 8; i++)
		vPoints[i] = XMLoadFloat4(&m_pPoints[i]);
#ifdef _DEBUG
	m_pGizmo->DrawCube(vPoints, pCameraTag, XMVectorSet(0.f, 0.f, 1.f, 1.f));
#endif
	return S_OK;
}

_uint CCullingBox::Update_Matrix(_fmatrix matTransform)
{
	//_vector vRight = XMVector3Normalize(matTransform.r[0]);
	//_vector vUp = XMVector3Normalize(matTransform.r[1]);
	//_vector vLook = XMVector3Normalize(matTransform.r[2]);
	//_matrix matMatrix;
	//matMatrix.r[0] = vRight;
	//matMatrix.r[1] = vUp;
	//matMatrix.r[2] = vLook;
	//matMatrix.r[3] = matTransform.r[3];

	for (_uint i = 0; i < 8; i++)
	{
		_vector vPoint = XMLoadFloat4(&m_pPoints[i]);
		vPoint = XMVector3TransformCoord(vPoint, matTransform);
		XMStoreFloat4(&m_pPoints[i], vPoint);
	}

	return _uint();
}

HRESULT CCullingBox::CreateWithPoints(_fvector vMin, _fvector vMax)
{
	_float fMinX, fMinY, fMinZ;
	_float fMaxX, fMaxY, fMaxZ;

	fMinX = XMVectorGetX(vMin);
	fMinY = XMVectorGetY(vMin);
	fMinZ = XMVectorGetZ(vMin);

	fMaxX = XMVectorGetX(vMax);
	fMaxY = XMVectorGetY(vMax);
	fMaxZ = XMVectorGetZ(vMax);

	_vector vPoint[8];
	vPoint[0] = vMin;
	vPoint[1] = XMVectorSet(fMaxX, fMinY, fMinZ,1.f);
	vPoint[2] = XMVectorSet(fMaxX, fMinY, fMaxZ,1.f);
	vPoint[3] = XMVectorSet(fMinX, fMinY, fMaxZ,1.f);

	vPoint[4] = XMVectorSet(fMinX, fMaxY, fMinZ,1.f);
	vPoint[5] = XMVectorSet(fMaxX, fMaxY, fMinZ,1.f);
	vPoint[6] = vMax;
	vPoint[7] = XMVectorSet(fMinX, fMaxY, fMaxZ,1.f);

	for (_uint i = 0; i < 8; i++)
		XMStoreFloat4(&m_pPoints[i], vPoint[i]);

	return S_OK;
}

HRESULT CCullingBox::CreateWithLenght(_float fWidth, _float fHeight, _float fDepth)
{
	_vector vPoints[8];

	for (_uint i = 0; i < 8; i++)
		vPoints[i] = XMLoadFloat4(&m_pPoints[i]);

	_float fMinX, fMinY, fMinZ;

	fMinX = XMVectorGetX(vPoints[0]);
	fMinY = XMVectorGetY(vPoints[0]);
	fMinZ = XMVectorGetZ(vPoints[0]);

	vPoints[1] = XMVectorSetX(vPoints[1], fMinX + fWidth);
	vPoints[2] = XMVectorSet(fMinX + fWidth, XMVectorGetY(vPoints[2]), fMinZ + fDepth,1.f);
	vPoints[3] = XMVectorSetZ(vPoints[3],fMinZ + fDepth);
	vPoints[4] = XMVectorSetY(vPoints[4], fMinY + fHeight);
	vPoints[5] = XMVectorSet(fMinX + fWidth, fMinY+fHeight, XMVectorGetZ(vPoints[5]),1.f);
	vPoints[6] = XMVectorSet(fMinX + fWidth, fMinY+fHeight, fMinZ+fDepth,1.f);
	vPoints[7] = XMVectorSet(XMVectorGetX(vPoints[7]), fMinY+fHeight, fMinZ+fDepth,1.f);

	for (_uint i = 0; i < 8; i++)
		XMStoreFloat4(&m_pPoints[i], vPoints[i]);

	return S_OK;
}

CCullingBox* CCullingBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CCullingBox* pInstance = new CCullingBox(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCullingBox Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCullingBox::Clone(void* pArg)
{
	CCullingBox* pInstance = new CCullingBox(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCullingBox Clone Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCullingBox::Free()
{
	CComponent::Free();
	Safe_Release(m_pGizmo);
}
