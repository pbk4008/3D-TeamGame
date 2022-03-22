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
		ERigidType eRigidType = ERigidType::Dynamic;
		_bool isGravity = false;
		_bool isKinematic = false;
		/* For.Shape */
		_bool isSceneQuery = false; // cct로 밀거나 물리작용하는 오브젝트들
		_bool isTrigger = false;
		/* For.Material */
		_float fStaticFriction = 0.5f;
		_float fDynamicFriction = 0.5f;
		_float fRestitution = 0.6f;
		/* For.Collider */
		CGameObject* pGameObject = nullptr;
	}DESC;
protected:
	explicit CCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCollider(const CCollider& _rhs);
	virtual ~CCollider() = default;

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