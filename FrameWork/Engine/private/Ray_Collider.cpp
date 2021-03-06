#include "Ray_Collider.h"
#include "Transform.h"
#include "PipeLine.h"


CRay_Collider::CRay_Collider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CRay_Collider::CRay_Collider(const CRay_Collider & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_pBatch(rhs.m_pBatch)
	, m_pInputLayout(rhs.m_pInputLayout)
	, m_vColor(rhs.m_vColor)
	, m_eColliderType(rhs.m_eColliderType)
{
	
	Safe_AddRef(m_pInputLayout);
}

_fvector CRay_Collider::Get_Min(BoundingBox* pBoundBox)
{
	return XMVectorSet(pBoundBox->Center.x - pBoundBox->Extents.x,
		pBoundBox->Center.y - pBoundBox->Extents.y,
		pBoundBox->Center.z - pBoundBox->Extents.z,
		1.f);
}

_fvector CRay_Collider::Get_Max(BoundingBox* pBoundBox)
{
	return XMVectorSet(pBoundBox->Center.x + pBoundBox->Extents.x,
		pBoundBox->Center.y + pBoundBox->Extents.y,
		pBoundBox->Center.z + pBoundBox->Extents.z,
		1.f);
}

HRESULT CRay_Collider::NativeConstruct_Prototype(TYPE eColliderType)
{
	m_eColliderType = eColliderType;

	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void*		pShaderByteCode = nullptr;
	size_t			ShaderByteCodeLength = 0;
	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pDeviceContext);

	return S_OK;
}

HRESULT CRay_Collider::NativeConstruct(void * pArg)
{
	COLLIDERDESC*		pColliderDesc = (COLLIDERDESC*)pArg;

	m_pTransform = CTransform::Create(m_pDevice, m_pDeviceContext);

 	m_pTransform->Scaling(XMLoadFloat3(&pColliderDesc->vScale));

	m_pTransform->SetUp_Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), pColliderDesc->vRotation.x);
	m_pTransform->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), pColliderDesc->vRotation.y);
	m_pTransform->SetUp_Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), pColliderDesc->vRotation.z);

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&pColliderDesc->vPosition));

	if (TYPE_AABB == m_eColliderType)
	{
		m_pOriginAABB = new BoundingBox(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(0.5f, 0.5f, 0.5f));
		m_pOriginAABB->Transform(*m_pOriginAABB, m_pTransform->Get_WorldMatrix());

		m_pAABB = new BoundingBox;
	}
	else if (TYPE_OBB == m_eColliderType)
	{
		m_pOriginOBB = new BoundingOrientedBox(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));
		m_pOriginOBB->Transform(*m_pOriginOBB, m_pTransform->Get_WorldMatrix());
		m_pOriginOBB->Extents = _float3(pColliderDesc->vScale.x * 0.5f, pColliderDesc->vScale.y * 0.5f, pColliderDesc->vScale.z * 0.5f);
		m_pOBB = new BoundingOrientedBox;

	}
	else
	{
		m_pOriginSphere = new BoundingSphere(XMFLOAT3(0.f, 0.f, 0.f), 0.5f);
		m_pOriginSphere->Transform(*m_pOriginSphere, m_pTransform->Get_WorldMatrix());
		m_pSphere = new BoundingSphere;
	}

	return S_OK;
}

void CRay_Collider::Update(_fmatrix TransformMatrix)
{
	/* ?????????????? ?????? ?????? ???????? ???????? ???????????? ????????. */
	/* ?????????????? ?????? ?????? ???????? ???????? ???????????? ???????? ??????. */
	_matrix		WorldMatrix = Remove_Scale(TransformMatrix);

	switch (m_eColliderType)
	{
	case CRay_Collider::TYPE_AABB:
		WorldMatrix = Remove_Rotation(TransformMatrix);
		m_pOriginAABB->Transform(*m_pAABB, WorldMatrix);
		break;
	case CRay_Collider::TYPE_OBB:
		m_pOriginOBB->Transform(*m_pOBB, WorldMatrix);
		break;
	case CRay_Collider::TYPE_SPHERE:
		m_pOriginSphere->Transform(*m_pSphere, WorldMatrix);
		break;
	}
}

HRESULT CRay_Collider::Render(const wstring& pCameraTag)
{	
	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	//m_vColor = m_isCollision == true ? _float4(1.f, 0.f, 0.f, 1.f) : _float4(0.f, 1.f, 0.f, 1.f);
	if (m_isCollision || m_isRaycast)
		m_vColor = _float4(1.f, 0.f, 0.f, 1.f);
	else
		m_vColor = _float4(0.f, 1.f, 0.f, 1.f);

	m_pEffect->SetView(pPipeLine->Get_Transform(pCameraTag,TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();

	switch (m_eColliderType)
	{
	case CRay_Collider::TYPE_AABB:
		DX::Draw(m_pBatch, *m_pAABB, XMLoadFloat4(&m_vColor));
		break;
	case CRay_Collider::TYPE_OBB:
		DX::Draw(m_pBatch, *m_pOBB, XMLoadFloat4(&m_vColor));
		break;
	case CRay_Collider::TYPE_SPHERE:
		DX::Draw(m_pBatch, *m_pSphere, XMLoadFloat4(&m_vColor));
		break;
	}

	m_pBatch->End();

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}

_bool CRay_Collider::Collision_AABB(CRay_Collider* pTargetCollider)
{
	if (TYPE_AABB != m_eColliderType ||
		TYPE_AABB != pTargetCollider->m_eColliderType)
		return false;

	_vector		vSourMin, vSourMax;
	_vector		vDestMin, vDestMax;

	vSourMin = Get_Min(m_pAABB);
	vSourMax = Get_Max(m_pAABB);

	vDestMin = Get_Min(pTargetCollider->m_pAABB);
	vDestMax = Get_Max(pTargetCollider->m_pAABB);

	m_isCollision = false;

	/* x???????????? ????. */
	if (max(XMVectorGetX(vSourMin), XMVectorGetX(vDestMin)) > 
		min(XMVectorGetX(vSourMax), XMVectorGetX(vDestMax)))
		return false;

	/* y???????????? ????. */
	if (max(XMVectorGetY(vSourMin), XMVectorGetY(vDestMin)) >
		min(XMVectorGetY(vSourMax), XMVectorGetY(vDestMax)))
		return false;

	/* z???????????? ????. */
	if (max(XMVectorGetZ(vSourMin), XMVectorGetZ(vDestMin)) >
		min(XMVectorGetZ(vSourMax), XMVectorGetZ(vDestMax)))
		return false;

	m_isCollision = true;

	return true;
}

_bool CRay_Collider::Collision_OBB(CRay_Collider* pTargetCollider)
{
	m_isCollision = false;

	OBB			OBBDesc[2];

	OBBDesc[0] = Compute_OBBDesc();
	OBBDesc[1] = pTargetCollider->Compute_OBBDesc();

	/* 0 : ?????? ?????? ???? ?????? ???????? ????. */
	/* 1 : A?????? ?????????????? ???????? ?????? ??. */
	/* 2 : B?????? ?????????????? ???????? ?????? ??. */
	_float		fDistance[3];

	_vector		vCenterDir = XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter);

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(vCenterDir, XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			if (fDistance[0] > fDistance[1] + fDistance[2])
				return false;
		}
	}
	m_isCollision = true;
	return true;
}

_bool CRay_Collider::Collision_Sphere(CRay_Collider* pTargetCollider)
{
	return _bool();
}

_bool CRay_Collider::Raycast_AABB(const _fvector& _svRayPos, const _fvector& _svRayDir, _float& _fOutDist)
{
	if (m_pAABB->Intersects(_svRayPos, _svRayDir, _fOutDist))
		m_isRaycast = true;
	else
		m_isRaycast = false;

	return m_isRaycast;
}

_fmatrix CRay_Collider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix		RemoveRotation = TransformMatrix;

	_vector		vScale = XMVectorSet(XMVectorGetX(XMVector3Length(TransformMatrix.r[0])),
		XMVectorGetX(XMVector3Length(TransformMatrix.r[1])),
		XMVectorGetX(XMVector3Length(TransformMatrix.r[2])), 
		0.f);

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(vScale);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetY(vScale);
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetZ(vScale);

	RemoveRotation.r[0] = vRight;
	RemoveRotation.r[1] = vUp;
	RemoveRotation.r[2] = vLook;

	return RemoveRotation;
}

_fmatrix CRay_Collider::Remove_Scale(_fmatrix TransformMatrix)
{
	_matrix		Matrix;
	Matrix = TransformMatrix;
	
	Matrix.r[0] = XMVector3Normalize(TransformMatrix.r[0]);
	Matrix.r[1] = XMVector3Normalize(TransformMatrix.r[1]);
	Matrix.r[2] = XMVector3Normalize(TransformMatrix.r[2]);	
	
	return Matrix;
}

CRay_Collider::OBB CRay_Collider::Compute_OBBDesc()
{
	OBB				OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBB));

	_float3			vPoint[8];
	m_pOBB->GetCorners(vPoint);

	/* ?????? ???? ????????. */
	XMStoreFloat3(&OBBDesc.vCenter,
		(XMLoadFloat3(&vPoint[0]) + XMLoadFloat3(&vPoint[6])) * 0.5f);

	/* ?????? ?????? ?? ?????? ????????. */
	XMStoreFloat3(&OBBDesc.vAlignAxis[0],
		XMVector3Normalize(XMLoadFloat3(&vPoint[2]) - XMLoadFloat3(&vPoint[3])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[1],
		XMVector3Normalize(XMLoadFloat3(&vPoint[2]) - XMLoadFloat3(&vPoint[1])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[2],
		XMVector3Normalize(XMLoadFloat3(&vPoint[2]) - XMLoadFloat3(&vPoint[6])));

	/* ?????? ???????? ?? ???? ?????? ???????? ???? ?????? ??????. */
	XMStoreFloat3(&OBBDesc.vCenterAxis[0],
		((XMLoadFloat3(&vPoint[2]) + XMLoadFloat3(&vPoint[5])) * 0.5f) - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[1],
		((XMLoadFloat3(&vPoint[2]) + XMLoadFloat3(&vPoint[7])) * 0.5f) - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[2],
		((XMLoadFloat3(&vPoint[2]) + XMLoadFloat3(&vPoint[0])) * 0.5f) - XMLoadFloat3(&OBBDesc.vCenter));

	return OBBDesc;
}

CRay_Collider* CRay_Collider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, TYPE eColliderType)
{
	CRay_Collider*		pInstance = new CRay_Collider(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eColliderType)))
	{
		MSGBOX("Failed to Creating CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRay_Collider::Clone(void * pArg)
{
	CRay_Collider*		pInstance = new CRay_Collider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRay_Collider::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete(m_pEffect);
		Safe_Delete(m_pBatch);
	}

	Safe_Release(m_pTransform);

	Safe_Release(m_pInputLayout);




	Safe_Delete(m_pOriginAABB);
	Safe_Delete(m_pOriginOBB);
	Safe_Delete(m_pOriginSphere);
	Safe_Delete(m_pSphere);
	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOBB);
}
