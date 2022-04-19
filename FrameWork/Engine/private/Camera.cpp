#include "Camera.h"
#include "PipeLine.h"

CCamera::CCamera()
	: m_tCameraDesc()
{
	ZeroMemory(&m_matView, sizeof(_float4x4));
	ZeroMemory(&m_matProj, sizeof(_float4x4));
}

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context)
	: CComponent(pDevice,pDevice_Context)
	, m_tCameraDesc()
{
	ZeroMemory(&m_matView, sizeof(_float4x4));
	ZeroMemory(&m_matProj, sizeof(_float4x4));
}

CCamera::CCamera(const CCamera& rhs)
	:CComponent(rhs)
	, m_matView(rhs.m_matView)
	, m_matProj(rhs.m_matProj)
	, m_tCameraDesc(rhs.m_tCameraDesc)
{
	
}

HRESULT CCamera::NativeConstruct_Prototype()
{
	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matProj, XMMatrixIdentity());

	return S_OK;
}

HRESULT CCamera::NativeConstruct(void* pArg)
{	
	if (nullptr == pArg)
	{
		return S_OK;
	}

	m_tCameraDesc = (*(CAMERADESC*)pArg);

	if (FAILED(setViewMatrix()))
		return E_FAIL;

	if (FAILED(setProjMatrix()))
		return E_FAIL;

	CPipeLine* pInstance = GET_INSTANCE(CPipeLine);

	if (FAILED(pInstance->Add_Camera(m_tCameraDesc.pCameraTag)))
	{
		RELEASE_INSTANCE(CPipeLine);
		return E_FAIL;
	};
	RELEASE_INSTANCE(CPipeLine);
	
	return S_OK;
}

void CCamera::Update_Matrix(const _fmatrix& matWorld)
{
	_matrix tmpWorld = matWorld;
	_matrix matView = XMLoadFloat4x4(&m_matView);
	_matrix matProj = XMLoadFloat4x4(&m_matProj);

	matView *= tmpWorld;

	CPipeLine* pInstance = GET_INSTANCE(CPipeLine);

	pInstance->Set_Transform(m_tCameraDesc.pCameraTag,TRANSFORMSTATEMATRIX::D3DTS_VIEW, XMMatrixInverse(nullptr, matView));
	pInstance->Set_Transform(m_tCameraDesc.pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION, matProj);

	RELEASE_INSTANCE(CPipeLine);
}

HRESULT CCamera::setViewMatrix()
{
	if (m_tCameraDesc.eType == CAMERATYPE::CAMERA_ORTHO)
	{
		XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
		return S_OK;
	}


	//EYE実特
	memcpy(m_matView.m[3], &m_tCameraDesc.vEye, sizeof(_float4));

	_float4 vLook,vUp,vRight;

	//Look実特
	_vector vTmpLook = XMLoadFloat4(&m_tCameraDesc.vAt) - XMLoadFloat4(&m_tCameraDesc.vEye);
	vTmpLook = XMVector3Normalize(vTmpLook);
	XMStoreFloat4(&vLook, vTmpLook);

	//Right実特
	_vector vTmpRight = XMVector3Cross(XMLoadFloat4(&m_tCameraDesc.vAxisY),vTmpLook);
	vTmpRight = XMVector3Normalize(vTmpRight);
	XMStoreFloat4(&vRight, vTmpRight);
	
	//Up実特
	_vector vTmpUp = XMVector3Cross(vTmpLook, vTmpRight);
	vTmpRight = XMVector3Normalize(vTmpUp);
	XMStoreFloat4(&vUp, vTmpUp);

	memcpy(m_matView.m[0], &vRight, sizeof(_float4));
	memcpy(m_matView.m[1], &vUp, sizeof(_float4));
	memcpy(m_matView.m[2], &vLook, sizeof(_float4));

	return S_OK;
}

HRESULT CCamera::setProjMatrix()
{
	if (m_tCameraDesc.eType == CCamera::CAMERATYPE::CAMERA_ORTHO)
	{
		XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH(m_tCameraDesc.fWinCX, m_tCameraDesc.fWinCY, m_tCameraDesc.fNear, m_tCameraDesc.fFar));
	}
	else if (m_tCameraDesc.eType == CCamera::CAMERATYPE::CAMERA_PROJECTION)
	{
		XMStoreFloat4x4(&m_matProj,XMMatrixPerspectiveFovLH(m_tCameraDesc.fFovy, m_tCameraDesc.fAspect, m_tCameraDesc.fNear, m_tCameraDesc.fFar));
	}

	return S_OK;
}

CCamera* CCamera::Clone(void* pArg)
{
	CCamera* pInstance = new CCamera(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCamera Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CCamera* CCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context)
{
	CCamera* pInstance = new CCamera(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCamera Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_uint CCamera::getType()
{
	return (_uint)m_tCameraDesc.eType;
}

void CCamera::setDesc(const CAMERADESC& _tDesc)
{
	m_tCameraDesc = _tDesc;

	setViewMatrix();
	setProjMatrix();
}

void CCamera::Free()
{
	CComponent::Free();
}
