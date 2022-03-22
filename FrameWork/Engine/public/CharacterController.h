#pragma once

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)

class CGameObject;
class CControllerFilterCallback;
class CTransform;
class CGizmo;

class ENGINE_DLL CCharacterController : public CComponent
{
	friend class CPhysicsXSystem;
public:
	typedef struct tagDesc
	{
		_float fHeight = 0.f;
		_float fRadius = 0.f;
		_float fContactOffset = 0.1f;
		_float fStepOffset = 0.f;
		_float fSlopeLimit = 30.f;
		_float fStaticFriction = 0.f;
		_float fDynamicFriction = 0.f;
		_float fRestitution = 0.f;
		_float3 vPosition = { 0.f, 0.f, 0.f };
		_float3 vUpDirection = { 0.f, 1.f, 0.f };
		CGameObject* pGameObject = nullptr;
		PxCapsuleClimbingMode::Enum eClimbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	}DESC;
private:
	explicit CCharacterController(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCharacterController(const CCharacterController& _rhs);
	virtual ~CCharacterController() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	const _int Tick(const _double& _dDeltaTime);
	const _int LateTick(const _double& _dDeltaTime);

public:
	const DESC& Get_CharacterControllerDesc() const;

	void setMaterial(PxMaterial* _pMateiral);
	void setPxController(PxController* _pPxController);
	void setShapes(vector<PxShape*>& _vecShapes);

	void setFootPosition(const _float3& _vPosition);
	void setOwnerTransform(CTransform* _pTransform);

	const _bool IsDown();
	const _bool IsUp();
	const _bool IsSides();
	
public:
	void Move(const _double& _dDeltaTime, const _float3 _vVelocity);
	void Update_OwnerTransform();

private:
	HRESULT Create_Controller();

	_fvector getQuaternion(_fmatrix matTransform);

private:
	DESC m_tDesc;

	PxControllerCollisionFlags m_preFlag;
	PxControllerCollisionFlags m_curFlag;

	PxFilterData m_filterData;
	CControllerFilterCallback* m_pFilterCallback = nullptr;
	PxController* m_pPxController = nullptr;
	vector<PxShape*> m_vecShapes;

	PxMaterial* m_pMaterial = nullptr;
	_float3 m_vVelocity = { 0.f, 0.f, 0.f };

	CTransform* m_pOwnerTransform = nullptr;

public:
	static CCharacterController* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CComponent* Clone(void* _pArg = nullptr);
	virtual void Free() override;
};

END