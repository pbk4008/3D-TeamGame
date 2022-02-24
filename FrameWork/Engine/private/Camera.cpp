#include "Camera.h"
#include "PipeLine.h"

CCamera::CCamera()
	: m_pCameraDesc(nullptr)
{
	ZeroMemory(&m_matView, sizeof(_float4x4));
	ZeroMemory(&m_matProj, sizeof(_float4x4));
}

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context)
	: CComponent(pDevice,pDevice_Context)
	, m_pCameraDesc(nullptr)
{
	ZeroMemory(&m_matView, sizeof(_float4x4));
	ZeroMemory(&m_matProj, sizeof(_float4x4));
}

CCamera::CCamera(const CCamera& rhs)
	:CComponent(rhs)
	, m_matView(rhs.m_matView)
	, m_matProj(rhs.m_matProj)
	, m_pCameraDesc(rhs.m_pCameraDesc)
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
	if (!m_isCloned)
	{
		m_pCameraDesc = new CAMERADESC;
		*m_pCameraDesc = *(CAMERADESC*)pArg;

		if (FAILED(setViewMatrix()))
			return E_FAIL;

		if (FAILED(setProjMatrix()))
			return E_FAIL;
		if(m_pCameraDesc->pCameraTag != L"")
		{
			CPipeLine* pInstance = GET_INSTANCE(CPipeLine);

			if (FAILED(pInstance->Add_Camera(m_pCameraDesc->pCameraTag)))
			{
				RELEASE_INSTANCE(CPipeLine);
				return E_FAIL;
			};
			RELEASE_INSTANCE(CPipeLine);
		}
	}
	else
	{
		CAMERADESC tmpDesc;
		memcpy(&tmpDesc, m_pCameraDesc, sizeof(CAMERADESC));
		if(tmpDesc.pCameraTag == L"")
		{
			const _tchar* pTag = (const _tchar*)pArg;
			tmpDesc.pCameraTag = pTag;
		}
		m_pCameraDesc = new CAMERADESC(tmpDesc);
		CPipeLine* pInstance = GET_INSTANCE(CPipeLine);
		if (FAILED(pInstance->Add_Camera(m_pCameraDesc->pCameraTag)))
		{
			RELEASE_INSTANCE(CPipeLine);
			return E_FAIL;
		};
		RELEASE_INSTANCE(CPipeLine);
	}
	return S_OK;
}

void CCamera::Update_Matrix(const _fmatrix& matWorld)
{
	_matrix tmpWorld = matWorld;
	_matrix matView = XMLoadFloat4x4(&m_matView);
	_matrix matProj = XMLoadFloat4x4(&m_matProj);

	matView *= tmpWorld;

	CPipeLine* pInstance = GET_INSTANCE(CPipeLine);

	pInstance->Set_Transform(m_pCameraDesc->pCameraTag,TRANSFORMSTATEMATRIX::D3DTS_VIEW, XMMatrixInverse(nullptr, matView));
	pInstance->Set_Transform(m_pCameraDesc->pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION, matProj);


	RELEASE_INSTANCE(CPipeLine);
}

HRESULT CCamera::setViewMatrix()
{
	if (m_pCameraDesc->eType == CAMERATYPE::CAMERA_ORTHO)
	{
		XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
		return S_OK;
	}


	//EYE実特
	memcpy(m_matView.m[3], &m_pCameraDesc->vEye, sizeof(_float4));

	_float4 vLook,vUp,vRight;

	//Look実特
	_vector vTmpLook = XMLoadFloat4(&m_pCameraDesc->vAt) - XMLoadFloat4(&m_pCameraDesc->vEye);
	vTmpLook = XMVector3Normalize(vTmpLook);
	XMStoreFloat4(&vLook, vTmpLook);

	//Right実特
	_vector vTmpRight = XMVector3Cross(XMLoadFloat4(&m_pCameraDesc->vAxisY),vTmpLook);
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
	if (m_pCameraDesc->eType == CCamera::CAMERATYPE::CAMERA_ORTHO)
	{
		XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH(m_pCameraDesc->fWinCX, m_pCameraDesc->fWinCY, m_pCameraDesc->fNear, m_pCameraDesc->fFar));
	}
	else if (m_pCameraDesc->eType == CCamera::CAMERATYPE::CAMERA_PROJECTION)
	{
		XMStoreFloat4x4(&m_matProj,XMMatrixPerspectiveFovLH(m_pCameraDesc->fFovy, m_pCameraDesc->fAspect, m_pCameraDesc->fNear, m_pCameraDesc->fFar));
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
	return (_uint)m_pCameraDesc->eType;
}

void CCamera::Free()
{
	CComponent::Free();
	Safe_Delete(m_pCameraDesc);
}
