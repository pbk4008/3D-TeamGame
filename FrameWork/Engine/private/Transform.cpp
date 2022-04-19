#include "..\public\Transform.h"
#include "GameObject.h"
#include "Navigation.h"

#include "CharacterController.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
	XMStoreFloat4x4(&m_matPivot, XMMatrixIdentity());
}

const _fvector CTransform::Get_CombinedState(const STATE _eState)
{
	_matrix smatCombined = XMLoadFloat4x4(&m_matPivot) * XMLoadFloat4x4(&m_WorldMatrix);
	_float4x4 matCombined; XMStoreFloat4x4(&matCombined, smatCombined);

	return XMLoadFloat4((_float4*)&matCombined.m[_eState][0]);
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));
	return S_OK;
}

const _int CTransform::Tick(_double& _dDeltaTime)
{
	return _int();
}

const _int CTransform::LateTick(_double& _dDeltaTime)
{
	return _int();
}

void CTransform::Set_TransformDesc(_float fSpeedPerSec, _float fAnglePerSec)
{
	m_TransformDesc.fSpeedPerSec = fSpeedPerSec;
	m_TransformDesc.fRotationPerSec = fAnglePerSec;
}

void CTransform::Set_PivotMatrix(const _fmatrix& _matPivot)
{
	XMStoreFloat4x4(&m_matPivot, _matPivot);
}

void CTransform::Go_Straight(_double TimeDelta, CNavigation * pNavigation)
{
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Straight(const _float _fVelocityZ)
{
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * _fVelocityZ;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_double TimeDelta)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vRight) * -1.f * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);	
}

void CTransform::Go_Right(_double TimeDelta)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Right(const _float _fVelocityX)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vRight) * _fVelocityX;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_BackWard(_double TimeDelta)
{
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * -1.f * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Up(_double TimeDelta)
{
	_vector		vUp = Get_State(STATE_UP);
	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vUp) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;
	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Up(const _float _fVelocityY)
{
	_vector		vUp = Get_State(STATE_UP);
	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vUp) * _fVelocityY;
	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Down(_double TimeDelta)
{
	_vector		vUp = Get_State(STATE_UP);
	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vUp) * m_TransformDesc.fSpeedPerSec * -(_float)TimeDelta;
	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Chase_Target(const CTransform * pTargetTransform, _double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = pTargetTransform->Get_State(CTransform::STATE_POSITION) - vPosition;

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	Face_Target(pTargetTransform->Get_State(CTransform::STATE_POSITION));

	Set_State(CTransform::STATE_POSITION, vPosition);
}

_fvector CTransform::Chase_Pos(const CTransform* pTargetTransform, _double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = pTargetTransform->Get_State(CTransform::STATE_POSITION) - vPosition;

	Face_Target(pTargetTransform->Get_State(CTransform::STATE_POSITION));
	return XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;
}

void CTransform::Face_Target(_fvector vTargetPos)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = vTargetPos - vPosition;

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	vLook = XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));

	vLook = XMVector3Normalize(vLook) * Get_Scale(CTransform::STATE_LOOK);
	vRight = XMVector3Normalize(vRight) * Get_Scale(CTransform::STATE_RIGHT);

	Set_State(CTransform::STATE_LOOK, vLook);
	Set_State(CTransform::STATE_RIGHT, vRight);
}


void CTransform::Rotation_Axis(_fvector vAxis, _double TimeDelta)
{
	if (XMVector3Equal(vAxis, _vector{ 0.f, 0.f, 0.f, 0.f }))
		return;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.fRotationPerSec * (_float)TimeDelta );

	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Rotation_Axis(const STATE _eState, const _float _fValue)
{
	_vector		svRight = XMVector3Normalize(Get_State(CTransform::STATE_RIGHT));
	_vector		svUp = XMVector3Normalize(Get_State(CTransform::STATE_UP));
	_vector		svLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK));
	_matrix		RotationMatrix = XMMatrixIdentity();

	switch (_eState)
	{
	case STATE_RIGHT:
		RotationMatrix = XMMatrixRotationAxis(svRight, _fValue);
		svUp = XMVector4Transform(svUp, RotationMatrix);
		svLook = XMVector4Transform(svLook, RotationMatrix);
		Set_State(CTransform::STATE_UP, svUp);
		Set_State(CTransform::STATE_LOOK, svLook);
		break;
	case STATE_UP:
		RotationMatrix = XMMatrixRotationAxis(svUp, _fValue);
		svRight = XMVector4Transform(svRight, RotationMatrix);
		svLook = XMVector4Transform(svLook, RotationMatrix);
		Set_State(CTransform::STATE_RIGHT, svRight);
		Set_State(CTransform::STATE_LOOK, svLook);
		break;
	case STATE_LOOK:
		RotationMatrix = XMMatrixRotationAxis(svLook, _fValue);
		svRight = XMVector4Transform(svRight, RotationMatrix);
		svUp = XMVector4Transform(svUp, RotationMatrix);
		Set_State(CTransform::STATE_RIGHT, svRight);
		Set_State(CTransform::STATE_UP, svUp);
		break;
	}
}

void CTransform::LookAt_RotYAxis(const _fvector& _dir)
{
	auto dir = _dir;
	XMVectorSetY(dir, 0);

	_float angle = acosf(XMVectorGetX(XMVector3Dot(dir, XMVectorSet(0.f, 0.f, 1.f, 0.f))));

	_vector rot;
	auto Temp = XMVector3Cross(dir, XMVectorSet(0.f, 0.f, 1.f, 0.f));

	if (XMVectorGetY(Temp) > 0)
		rot = XMQuaternionRotationRollPitchYaw(-angle, 0, 0);
	else
		rot = XMQuaternionRotationRollPitchYaw(angle, 0, 0);
}

void CTransform::SetUp_Rotation(_fvector vAxis, _float fRadian)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scale(CTransform::STATE_RIGHT);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scale(CTransform::STATE_UP);
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scale(CTransform::STATE_LOOK);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::SetUp_Rotation(const _float3& _vEuler)
{
	_vector svRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scale(STATE_RIGHT);
	_vector svUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scale(STATE_UP);
	_vector svLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scale(STATE_LOOK);

	_matrix smatRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(_vEuler.x), XMConvertToRadians(_vEuler.y), XMConvertToRadians(_vEuler.z));

	svRight = XMVector4Transform(svRight, smatRot);
	svUp = XMVector4Transform(svUp, smatRot);
	svLook = XMVector4Transform(svLook, smatRot);

	Set_State(STATE_RIGHT, svRight);
	Set_State(STATE_UP, svUp);
	Set_State(STATE_LOOK, svLook);
}

void CTransform::SetUp_Rotation(const _fvector vQuaternion)
{
	_float fSizeX = Get_Scale(CTransform::STATE_RIGHT);
	_float fSizeY = Get_Scale(CTransform::STATE_UP);
	_float fSizeZ = Get_Scale(CTransform::STATE_LOOK);

	_matrix matRotate = XMMatrixRotationQuaternion(vQuaternion);
	
	Set_State(CTransform::STATE_RIGHT, matRotate.r[0]*fSizeX);
	Set_State(CTransform::STATE_UP, matRotate.r[1]*fSizeY);
	Set_State(CTransform::STATE_LOOK, matRotate.r[2]*fSizeZ);
}

void CTransform::Scaling(_fvector vScale)
{
	_vector		vRight = XMVector3Normalize(Get_State(CTransform::STATE_RIGHT)) * XMVectorGetX(vScale);
	_vector		vUp = XMVector3Normalize(Get_State(CTransform::STATE_UP)) * XMVectorGetY(vScale);
	_vector		vLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK)) * XMVectorGetZ(vScale);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Scale_Up(_fvector vScale)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT) * XMVectorGetX(vScale);
	_vector		vUp = Get_State(CTransform::STATE_UP) * XMVectorGetY(vScale);
	_vector		vLook = Get_State(CTransform::STATE_LOOK) * XMVectorGetZ(vScale);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::ScaleX_Up(_fvector vScale)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT) * XMVectorGetX(vScale);
	Set_State(CTransform::STATE_RIGHT, vRight);
}

void CTransform::ScaleY_Up(_fvector vScale)
{
	_vector		vUp = Get_State(CTransform::STATE_UP) * XMVectorGetY(vScale);
	Set_State(CTransform::STATE_UP, vUp);
}

void CTransform::ScaleZ_Up(_fvector vScale)
{
	_vector		vLook = Get_State(CTransform::STATE_LOOK) * XMVectorGetZ(vScale);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Scale_One()
{
	_matrix matWorld = XMLoadFloat4x4(&m_WorldMatrix);

	_vector vRight = XMVector3Normalize(matWorld.r[0]);
	_vector vUp = XMVector3Normalize(matWorld.r[1]);
	_vector vLook = XMVector3Normalize(matWorld.r[2]);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

}

void CTransform::Fall(_double dDeltaTime)
{
	_vector svPos = Get_State(CTransform::STATE_POSITION);
	_float fY = XMVectorGetY(svPos);

	if (fY > -5.f)
		Add_Velocity(XMVectorSet(0.f, -9.8f * (_float)dDeltaTime, 0.f, 0.f));
}

void CTransform::Mesh_Straight(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * 0.5f * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Mesh_Left(_double TimeDelta)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vRight) * -1.f * 0.5f * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Mesh_Right(_double TimeDelta)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vRight) * 0.5f * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Mesh_BackWard(_double TimeDelta)
{
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * -1.f * 0.5f * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Mesh_Up(_double TimeDelta)
{
	_vector		vUp = Get_State(STATE_UP);
	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vUp) * 0.5f * (_float)TimeDelta;
	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Mesh_Down(_double TimeDelta)
{
	_vector		vUp = Get_State(STATE_UP);
	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vUp) * 0.5f * -(_float)TimeDelta;
	Set_State(STATE_POSITION, vPosition);
}

const _float3& CTransform::Get_Velocity() const
{
	return m_vVelocity;
}

void CTransform::Set_Velocity(const _fvector & _svVelocity)
{
	XMStoreFloat3(&m_vVelocity, _svVelocity);
}

void CTransform::Add_Velocity(const _fvector& _svVelocity)
{
	XMStoreFloat3(&m_vVelocity, XMLoadFloat3(&m_vVelocity) + _svVelocity);
}

void CTransform::Add_Velocity(const STATE _eState, const _float _fValue)
{
	_vector svVelocity = XMVectorZero();
	switch (_eState)
	{
	case STATE_RIGHT:
		svVelocity = XMVectorSet(_fValue, 0.f, 0.f, 0.f);
		break;
	case STATE_UP:
		svVelocity = XMVectorSet(0.f, _fValue, 0.f, 0.f);
		break;
	case STATE_LOOK:
		svVelocity = XMVectorSet(0.f, 0.f, _fValue, 0.f);
		break;
	}

	_vector svScale, svRot, svPos;
	XMMatrixDecompose(&svScale, &svRot, &svPos, XMLoadFloat4x4(&m_WorldMatrix));
	_matrix smatRotation = XMMatrixRotationQuaternion(svRot);
	svVelocity = XMVector4Transform(svVelocity, smatRotation);
	XMStoreFloat3(&m_vVelocity, XMLoadFloat3(&m_vVelocity) + svVelocity);
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTransform*		pInstance = new CTransform(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTransform");
		Safe_Release(pInstance);
	}		

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*		pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}