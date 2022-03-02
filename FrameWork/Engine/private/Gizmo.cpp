#include "Gizmo.h"
#include "PipeLine.h"

CGizmo::CGizmo()
	: m_pBatch(nullptr)
	, m_pEffect(nullptr)
	, m_pInputLayOut(nullptr)
{
}

CGizmo::CGizmo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
	, m_pBatch(nullptr)
	, m_pEffect(nullptr)
	, m_pInputLayOut(nullptr)
{
}

CGizmo::CGizmo(const CGizmo& rhs)
	:CComponent(rhs)
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayOut(rhs.m_pInputLayOut)
{
	Safe_AddRef(m_pInputLayOut);
}

HRESULT CGizmo::NativeConstruct_Prototype()
{
	if (FAILED(CComponent::NativeConstruct_Prototype()))
		return E_FAIL;

	if (!m_pDevice)
		return E_FAIL;
	
	m_pEffect = new BasicEffect(m_pDevice);
	if (!m_pEffect)
		return E_FAIL;

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		pShaderByteCode, ShaderByteCodeLength, &m_pInputLayOut)))
		return E_FAIL;

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pDeviceContext);

	return S_OK;
}

HRESULT CGizmo::NativeConstruct(void* pArg)
{
	return S_OK;
}

HRESULT CGizmo::DrawCube(_matrix matTransform, const wstring& pCameraTag, _fvector vColor)
{
	if (!m_pBatch)
		return E_FAIL;

	CPipeLine* pInstance = GET_INSTANCE(CPipeLine);

	m_pEffect->SetView(pInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	m_pEffect->SetProjection(pInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pDeviceContext->IASetInputLayout(m_pInputLayOut);

	m_pEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();

	_vector verts[8] =
	{
		{ -1.f, -1.f, -1.f, 0.f },
		{  1.f, -1.f, -1.f, 0.f },
		{  1.f, -1.f,  1.f, 0.f },
		{ -1.f, -1.f,  1.f, 0.f },
		{ -1.f,  1.f, -1.f, 0.f },
		{  1.f,  1.f, -1.f, 0.f },
		{  1.f,  1.f,  1.f, 0.f },
		{ -1.f,  1.f,  1.f, 0.f }
	};

	_ushort sIndices[] =
	{
		 0, 1,
		 1, 2,
		 2, 3,
		 3, 0,
		 4, 5,
		 5, 6,
		 6, 7,
		 7, 4,
		 0, 4,
		 1, 5,
		 2, 6,
		 3, 7
	};

	VertexPositionColor vtxPC[8];
	for (_uint i = 0; i < 8; i++)
	{
		_vector v = XMVector3Transform(verts[i], matTransform);
		XMStoreFloat3(&vtxPC[i].position, v);
		XMStoreFloat4(&vtxPC[i].color, vColor);
	}

	m_pBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_LINELIST, sIndices, _countof(sIndices), vtxPC, 8);

	m_pBatch->End();

	return S_OK;
}

HRESULT CGizmo::DrawLine(_fvector vStart, _fvector vEnd, const wstring& pCameraTag, _fvector vColor)
{
	if (!m_pBatch)
		return E_FAIL;

	CPipeLine* pInstance = GET_INSTANCE(CPipeLine);

	m_pEffect->SetView(pInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	m_pEffect->SetProjection(pInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pDeviceContext->IASetInputLayout(m_pInputLayOut);

	m_pEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();

	VertexPositionColor verts[2];

	XMStoreFloat3(&verts[0].position, vStart);
	XMStoreFloat4(&verts[0].color, XMVectorSet(1.f, 0.f, 0.f, 1.f));
	XMStoreFloat3(&verts[1].position, vEnd);
	XMStoreFloat4(&verts[1].color, XMVectorSet(1.f, 0.f, 0.f, 1.f));

	m_pBatch->DrawLine(verts[0], verts[1]);

	m_pBatch->End();

	return S_OK;
}

HRESULT CGizmo::DrawSphere(_matrix matTransform, const wstring& pCameraTag, _fvector vColor)
{
	if (!m_pBatch)
		return E_FAIL;

	CPipeLine* pInstance = GET_INSTANCE(CPipeLine);

	m_pEffect->SetView(pInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	m_pEffect->SetProjection(pInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pDeviceContext->IASetInputLayout(m_pInputLayOut);

	m_pEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();

	_vector vPos = matTransform.r[3];

	_float xSize = XMVectorGetX(XMVector3Length(matTransform.r[0]));
	_float ySize = XMVectorGetX(XMVector3Length(matTransform.r[1]));
	_float zSize = XMVectorGetX(XMVector3Length(matTransform.r[2]));

	XMVECTOR xaxis = g_XMIdentityR0 * xSize;
	XMVECTOR yaxis = g_XMIdentityR1 * ySize;
	XMVECTOR zaxis = g_XMIdentityR2 * zSize;

	DX::DrawRing(m_pBatch, vPos, xaxis, zaxis, vColor);
	DX::DrawRing(m_pBatch, vPos, xaxis, yaxis, vColor);
	DX::DrawRing(m_pBatch, vPos, yaxis, zaxis, vColor);
	
	m_pBatch->End();

	return S_OK;
}

HRESULT CGizmo::DrawCapsule(_matrix matTransform, const wstring& pCameraTag, _fvector vColor)
{
	if (!m_pBatch)
		return E_FAIL;

	CPipeLine* pInstance = GET_INSTANCE(CPipeLine);

	m_pEffect->SetView(pInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	m_pEffect->SetProjection(pInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pDeviceContext->IASetInputLayout(m_pInputLayOut);

	m_pEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();

	_vector vPos = matTransform.r[3];

	_matrix  matRotate = XMMatrixIdentity();

	_vector vRight = XMVector4Normalize(matTransform.r[0]);
	_vector vUp = XMVector4Normalize(matTransform.r[1]);
	_vector vLook = XMVector4Normalize(matTransform.r[2]);
	
	matRotate.r[0] = vRight;
	matRotate.r[1] = vUp;
	matRotate.r[2] = vLook;
	matRotate.r[3] = g_XMIdentityR3;

	_float fSphereRadius = XMVectorGetX(XMVector3Length(matTransform.r[0]));

	_vector xaxis = XMVector3Transform(g_XMIdentityR0 * fSphereRadius, matRotate);
	_vector zaxis = XMVector3Transform(g_XMIdentityR2 * fSphereRadius, matRotate);

	DX::DrawRing(m_pBatch, vPos, xaxis, zaxis, vColor);

	m_pBatch->End();

	matRotate.r[3] = vPos;

	_matrix matUpSphere, matDownSphere;
	matUpSphere = XMMatrixIdentity();
	_vector vUpSpherePos = g_XMIdentityR3;
	_float fHeight = XMVectorGetX(XMVector3Length(matTransform.r[1]));
	vUpSpherePos = XMVectorSetY(vUpSpherePos, fHeight * 0.5f);
	matUpSphere.r[3] = vUpSpherePos;

	matUpSphere *= matRotate;

	DrawSphere(matUpSphere, pCameraTag, vColor);

	matDownSphere = XMMatrixIdentity();
	_vector vDownSpherePos = g_XMIdentityR3;
	vDownSpherePos = XMVectorSetY(vDownSpherePos, -fHeight * 0.5f);
	matDownSphere.r[3] = vDownSpherePos;

	matDownSphere *= matRotate;
	DrawSphere(matDownSphere, pCameraTag, vColor);


	_vector vLeftStart, vLeftEnd, vRightStart, vRightEnd;

	_vector vSphereXAxis = XMVector3Normalize(matUpSphere.r[0]);
	_float fRadius = XMVectorGetX(XMVector3Length(matUpSphere.r[0]));

	vLeftStart = matUpSphere.r[3] - fRadius * vSphereXAxis;
	vLeftEnd = matDownSphere.r[3] - fRadius * vSphereXAxis;

	vRightStart = matUpSphere.r[3] + fRadius * vSphereXAxis;
	vRightEnd = matDownSphere.r[3] + fRadius * vSphereXAxis;

	DrawLine(vLeftStart, vLeftEnd, pCameraTag, vColor);
	DrawLine(vRightStart, vRightEnd, pCameraTag, vColor);

	return S_OK;
}

CGizmo* CGizmo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CGizmo* pInstance = new CGizmo(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CGizmo Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CGizmo::Clone(void* pArg)
{
	CGizmo* pInstance = new CGizmo(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CGizmo Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGizmo::Free()
{
	CComponent::Free();

	if (!m_isCloned)
	{
		Safe_Delete(m_pEffect);
		Safe_Delete(m_pBatch);
	}
	Safe_Release(m_pInputLayOut);
}
