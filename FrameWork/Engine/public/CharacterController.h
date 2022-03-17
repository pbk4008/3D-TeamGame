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
	typedef struct tagCharacterControllerDesc
	{
		_float fHeight = 0.f;
		_float fRadius = 0.f;
		_float fContactOffset = 0.1f;
		_float fStepOffset = 0.1f;
		_float fSlopeLimit = 30.f;
		_float fStaticFriction = 0.f;
		_float fDynamicFriction = 0.f;
		_float fRestitution = 0.f;
		_float3 vPosition = { 0.f, 0.f, 0.f };
		_float3 vUpDirection = { 0.f, 1.f, 0.f };
		CGameObject* pGameObject = nullptr;
		PxCapsuleClimbingMode::Enum eClimbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	}CHARACTERCONTROLLERDESC;
private:
	explicit CCharacterController(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCharacterController(const CCharacterController& _rhs);
	virtual ~CCharacterController() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	const _int Tick(const _double& _dDeltaTime);
	const _int LateTick(const _double& _dDeltaTime);
	HRESULT Render();

public:
	const CHARACTERCONTROLLERDESC& Get_CharacterControllerDesc() const;
	PxMaterial* Get_Material();

	void Set_OwnerTransform(CTransform* _pTransform);

	const _bool IsDown();
	const _bool IsUp();
	const _bool IsSides();
	
public:
	void Move(const _double& _dDeltaTime, const _float3 _vVelocity);
	void Update_OwnerTransform();

private:
	HRESULT Create_Controller();

	_fvector getQuaternion(_fmatrix matTransform);
	_fvector ToXMVector4(const PxQuat pxquat);
	const PxVec3 ToPxVector(_fvector xmvec);

	_fmatrix Update_Scale(_fmatrix matTransform);
	_fmatrix Update_Rotate(_fmatrix matTransform);
	_fmatrix Update_Position(PxVec3 vPos);

private:
	CHARACTERCONTROLLERDESC m_tCharacterControllerDesc;

	PxControllerCollisionFlags m_preFlag;
	PxControllerCollisionFlags m_curFlag;

	PxFilterData m_filterData;
	CControllerFilterCallback* m_pFilterCallback = nullptr;
	PxController* m_pPxController = nullptr;
	vector<PxShape*> m_vecShapes;

	PxMaterial* m_pMaterial = nullptr;
	_float3 m_vVelocity = { 0.f, 0.f, 0.f };

	CGizmo* m_pGizmo = nullptr;
	_float4x4 m_matLocal;
	_float4x4 m_matWorld;

	CTransform* m_pOwnerTransform = nullptr;

	// 그리기용
private:
	BasicEffect* m_pEffect = nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;

public:
	static CCharacterController* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CComponent* Clone(void* _pArg = nullptr);
	virtual void Free() override;
};

END