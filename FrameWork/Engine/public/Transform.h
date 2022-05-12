#pragma once

#include "Component.h"

/* 객체의 월드공간상에서의 상태.(WorldMatrix) */
/* 모든 정점들의 월드변환. */

BEGIN(Engine)

class CCharacterController;

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
public:
	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}TRANSFORMDESC;
private:
	explicit CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;
public:
	_fvector Get_State(STATE eState) const {	return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]); }
	const _fvector Get_CombinedState(const STATE _eState);




	_float Get_Scale(STATE eState) const { return XMVectorGetX(XMVector3Length(Get_State(eState))); }
	_fmatrix Get_WorldMatrix() const { return XMLoadFloat4x4(&m_WorldMatrix); }
	_fmatrix Get_WorldMatrixInverse() const { return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)); }
	const _fmatrix Get_PivotMatrix() const { return XMLoadFloat4x4(&m_matPivot); }
	const _fmatrix Get_CombinedMatrix() const { return XMLoadFloat4x4(&m_matPivot) * XMLoadFloat4x4(&m_WorldMatrix); }

	void Set_WorldMatrix(_fmatrix matWorld) { XMStoreFloat4x4(&m_WorldMatrix, matWorld); }
	void Set_State(STATE eState, _fvector vState) {	XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState][0], vState); }		
	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc) { m_TransformDesc = TransformDesc; }
	void Set_TransformDesc(_float fSpeedPerSec, _float fAnglePerSec);
	void Set_PivotMatrix(const _fmatrix& _matPivot);
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual const _int Tick(_double& _dDeltaTime);
	virtual const _int LateTick(_double& _dDeltaTime);

public:
	void Go_Straight(_double TimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Straight(const _float _fVelocityZ);
	void Go_Left(_double TimeDelta);
	void Go_Right(_double TimeDelta);
	void Go_Right(const _float _fVelocityX);
	void Go_BackWard(_double TimeDelta);
	void Go_Up(_double TimeDelta);
	void Go_Up(const _float _fVelocityY);
	void Go_Down(_double TimeDelta);
	void Chase_Target(const CTransform* pTargetTransform, _double TimeDelta);
	_fvector Chase_Pos(const CTransform* pTargetTransform, _double TimeDelta);

	void Face_Target(_fvector	vTargetPos);
	void Rotation_Axis(_fvector vAxis, _double TimeDelta); /* 동적으로 TimeDelta이용한 회전. */
	void Rotation_Axis(const STATE _eState, const _float _fValue);
	void LookAt_RotYAxis(const _fvector& _dir);
	void SetUp_Rotation(_fvector vAxis, _float fRadian); /* 디폴트상태기준으로 특정축으로 몇도만큼 회전시켜놓겠다. */
	void SetUp_Rotation(const _float3& _vEuler);
	void SetUp_Rotation(const _fvector vQuaternion);
	void Scaling(_fvector vScale);
	void Scale_Up(_fvector vScale);
	void ScaleX_Up(_fvector vScale);
	void ScaleY_Up(_fvector vScale);
	void ScaleZ_Up(_fvector vScale);
	void Scale_One();
public:
	void Fall(_double dDeltaTime);
	void SetPosY(_float fPosY);
	void SetPosX(_float fPosX);

public:
	void Mesh_Straight(_double TimeDelta, class CNavigation* pNavigation = nullptr);
	void Mesh_Left(_double TimeDelta);
	void Mesh_Right(_double TimeDelta);
	void Mesh_BackWard(_double TimeDelta);
	void Mesh_Up(_double TimeDelta);
	void Mesh_Down(_double TimeDelta);

public: /* For.CharacterController */
	const _float3& Get_Velocity() const;
	void Set_Velocity(const _fvector& _svVelocity);
	void Add_Velocity(const _fvector& _svVelocity);
	void Add_Velocity(const STATE _eState, const _float _fValue);

private:
	_float4x4				m_WorldMatrix;	
	_float4x4				m_matPivot;
	TRANSFORMDESC			m_TransformDesc;

	_float3					m_vVelocity = { 0.f, 0.f, 0.f };
public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END