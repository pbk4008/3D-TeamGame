#pragma once

#include "Component.h"
#include "PhysicsXSystem.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CCollider abstract : public CComponent
{
public:
	typedef struct tagDesc
	{
		/* For.Rigidbody */
		ERigidType eRigidType = ERigidType::Dynamic;			// 리지드스태틱(움직이지 않는 고정된 액터) or 리지드바디(움직이며 물리연산을 받는 액터)
		_bool isGravity = false;								// 중력을 받을것인가?
		_bool isKinematic = false;								// 다른 액터로부터 물리연산을 안받는 객체
		/* For.Shape */
		_bool isSceneQuery = false;								// cct로 밀거나 물리작용하는 오브젝트들
		_bool isTrigger = false;								// 트리거 로 만들 것인가?
		/* For.Material */
		_float fStaticFriction = 0.5f;							// 정적 마찰
		_float fDynamicFriction = 0.5f;							// 동적 마찰
		_float fRestitution = 0.6f;								// 물체가 튕기는 정도(?)
		/* For.Collider */
		CGameObject* pGameObject = nullptr;						// 액터를 가지고 있는 오브젝트
	}DESC;
protected:
	explicit CCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCollider(const CCollider& _rhs);
	virtual ~CCollider() = default;

public: void			Remove_Actor();

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual const _int Tick(const _double& _dDeltaTime);
	virtual const _int LateTick(const _double& _dDeltaTime);

	CGameObject* getGameObject();

	void setRigidActor(PxRigidActor* _pRigidbody);
	void setShape(PxShape* _pShape);
	void setMaterial(PxMaterial* _pMaterial);
	void setPivotMatrix(const _fmatrix& _smatPivot);

	_int Update_Transform();
	_int Update_PxTransform();

	const _bool Raycast(const _float3& _vOrigin, const _float3& _vDir, const _float _fMaxDistance, CGameObject** _ppOutHitObject);

protected:
	CPhysicsXSystem* m_pPhsyX = nullptr;
	PxRigidActor* m_pRigidActor = nullptr;
	PxShape* m_pShape = nullptr;
	PxMaterial* m_pMaterial = nullptr;
	CGameObject* m_pGameObject = nullptr;

	_float4x4 m_matPivot;


public:
	virtual void Free() override;
};

END