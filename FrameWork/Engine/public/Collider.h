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
		_bool isGravity = false;								// �߷��� �������ΰ�?
		_bool isKinematic = false;								// �ٸ� ���ͷκ��� ���������� �ȹ޴� ��ü
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