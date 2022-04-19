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
		ERigidType eRigidType = ERigidType::Dynamic;			// �����彺��ƽ(�������� �ʴ� ������ ����) or ������ٵ�(�����̸� ���������� �޴� ����)
		_bool isVisualization = true;							// ���� �Ұ��ΰ�?
		_bool isGravity = false;								// �߷��� �������ΰ�?
		_bool isKinematic = false;								// �ٸ� ���ͷκ��� ���������� �ȹ޴� ��ü
		_float fMass = 1.f;
		/* For.Shape */
		_bool isSceneQuery = false;								// cct�� �аų� �����ۿ��ϴ� ������Ʈ��
		_bool isTrigger = false;								// Ʈ���� �� ���� ���ΰ�?
		/* For.Material */
		_float fStaticFriction = 0.5f;							// ���� ����
		_float fDynamicFriction = 0.5f;							// ���� ����
		_float fRestitution = 0.6f;								// ��ü�� ƨ��� ����(?)
		/* For.Collider */
		CGameObject* pGameObject = nullptr;						// ���͸� ������ �ִ� ������Ʈ
	}DESC;
protected:
	explicit CCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCollider(const CCollider& _rhs);
	virtual ~CCollider() = default;

public:
	void Remove_ActorFromScene();
	void Add_ActorToScene();
	void Sleep();
	void WakeUp();
	void Add_Force(_fvector vPow);
	void Add_Torque(_fvector vAxis, _float fAngle);
	void Reset_Power();
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual const _int Tick(const _double& _dDeltaTime);
	virtual const _int LateTick(const _double& _dDeltaTime);

	const DESC& getDesc() const;
	CGameObject* getGameObject();

	void setRigidActor(PxRigidActor* _pRigidbody);
	void setShape(PxShape* _pShape);
	void setMaterial(PxMaterial* _pMaterial);
	void setMass(const _float _fValue);
	void setPivotMatrix(const _fmatrix& _smatPivot);

	void setGravity(const _bool _isGravity);
	void setKinematic(const _bool _isKinematic);
	void setTrigger(const _bool _isTrigger);
	void setSceneQuery(const _bool _isSceneQuery);
	
	void setShapeLayer(const _uint _iLayer);

	_int Update_Transform();
	_int Update_PxTransform();

protected:
	CPhysicsXSystem* m_pPhsyX = nullptr;
	PxRigidActor* m_pRigidActor = nullptr;
	PxShape* m_pShape = nullptr;
	PxMaterial* m_pMaterial = nullptr;
	CGameObject* m_pGameObject = nullptr;

	DESC m_tDesc;
	_float4x4 m_matPivot;


public:
	virtual void Free() override;
};

END