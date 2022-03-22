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
		_float fHeight = 0.f;								// 가운데 기둥 길이
		_float fRadius = 0.f;								// 기둥 넓이와 위아래 구의 반지름
		_float fContactOffset = 0.1f;						// 접촉 오프셋 : 0.f 로 설정시 터짐, 공식적으로  최소 0.01 ~ 반지름의 1/10 크기를 권장함
		_float fStepOffset = 0.f;							// 계단 높이와 같이 어느정도 높이는 그냥 올라가지게 만드는 수치
		_float fSlopeLimit = 30.f;							// 오를수 있는 경사의 한계수치
		/* For.Material */
		_float fStaticFriction = 0.5f;						// 정적 마찰
		_float fDynamicFriction = 0.5f;						// 동적 마찰
		_float fRestitution = 0.6f;							//	부딛혔을때 튕기는 탄성력(?)
		/*  */
		_float3 vPosition = { 0.f, 0.f, 0.f };				// 오프셋 : 설정안해줘도 됨.
		_float3 vUpDirection = { 0.f, 1.f, 0.f };			// 업벡터 : 플레이어나 몬스터는 업벡터가 대부분 0, 1, 0
		CGameObject* pGameObject = nullptr;					// 오너
		PxCapsuleClimbingMode::Enum eClimbingMode = PxCapsuleClimbingMode::eCONSTRAINED;		// 등반모드 
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