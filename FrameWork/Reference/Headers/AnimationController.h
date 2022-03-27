#pragma once

#include "Component.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CModel;
class CAnimation;
class CChannel;
class CCharacterController;

class ENGINE_DLL CAnimationController final : public CComponent
{
public:
	enum class EType { Transform, CharacterController, Max };
	typedef struct tagAnimBlendDesc
	{
		_bool isLoopNextAnim = true;
		_int iCurAnimIndex = 0;
		_int iNextAnimIndex = -1;
		_float fTakeTime = 0.2f;
		_float fChangeTime = 0.f;
		_float fTweenTime = 0.f;
	}ANIMBLENDDESC;
private:
	explicit CAnimationController(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CAnimationController(const CAnimationController& _rhs);
	virtual ~CAnimationController() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime, const EType _eType = EType::Transform);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	const _uint Get_CurAnimIndex() const;
	const _uint Get_CurKeyFrameIndex() const;
	const _uint Get_CurFixedBoneKeyFrameIndex() const;
	const _uint Get_MaxKeyFrameIndex() const;
	const ERootOption Get_RootOption() const;
	const _bool Get_ChangeAnimation() const;
	/* For.Debug */ // 외부에서 디버깅용으로 상태를 알아가기 위한 겟함수들
	const string& Get_CurAnimTag() const;

	void Set_GameObject(CGameObject* _pGameObject);
	void Set_Transform(CTransform* _pTransform);
	void Set_Model(CModel* _pModel);
	void Set_LoopAnim(const _bool _isLoop = true);
	void Set_TransformMove(const _bool _isTransformMove);
	void Set_PivotMatrix(const _fmatrix& _smatPivot);
	void Set_RootMotion(const _bool _isRootMotion, const _bool _isTransformMove, const ERootOption _eRootOption = ERootOption::XYZ);
	void Set_PlaySpeed(const _float _fPlaySpeed);
	void Set_MoveSpeed(const _float _fMoveSpeed);
	void Set_RotSpeed(const _float _fRotSpeed);
	void Set_IsChange(const _bool _bChange);
	const _bool Is_RootMotion() const;
	const _bool Is_Finished() const;

	void Add_TrackAcc(const _double& _dTrackAcc);

public:
	_int Update_CombinedTransformMatrix(const _double& _dDeltaTime);
	void Lerp_Anim(vector<CAnimation*>& _vecAnimations);
	HRESULT SetUp_NextAnimation(const string& _strAnimTag, const _bool _isLoopNextAnim = true);
	HRESULT SetUp_NextAnimation(_uint iIndex, const _bool _isLoopNextAnim = true);
	//다음 애니메이션 변경(AnimNode 직접 받도록)
	HRESULT SetUp_NextAnimation(class CAnimNode* pChangeAnimNode);
	const _int Move_Transform(const _double& _dDeltaTime);
	const _int Add_TransformVelocity(const _double& _dDeltaTime);
	void Reset_Animation();
	
public: /* For.Debug */
	void Render_Debug();

private:
	CGameObject* m_pGameObject = nullptr;
	CTransform* m_pTransform = nullptr;
	CModel* m_pModel = nullptr;

	string m_strPreAnimTag = "";
	string m_strCurAnimTag = "";

	CAnimation* m_pPreAnim = nullptr;
	CAnimation* m_pCurAnim = nullptr;

	CChannel* m_pRootBone = nullptr;
	CChannel* m_pFixedBone = nullptr;

	_bool m_isFinished = false;
	_bool m_isRootMotion = false;
	_bool m_isTransformMove = false;
	_bool m_isLoopAnim = true;
	_bool m_isChangeAnim = false;
	ERootOption m_eRootOption = ERootOption::XYZ;

	_int m_iPreFixedBoneKeyFrameIndex = 0;
	_int m_iCurFixedBoneKeyFrameIndex = 0;
	_uint m_iCurKeyFrameIndex = 0;
	_uint m_iMaxKeyFrameIndex = 0;

	_float m_fPlaySpeed = 1.f;
	_float m_fMoveSpeed = 1.f;
	_float m_fRotSpeed = 1.f;
	_float m_fFixedBoneHoldTime = 0.f;

	_float4x4 m_matPivot;

	ANIMBLENDDESC m_tBlendDesc;

	// 디버그용
	wstring m_wstrFixedBonePosition = L"";
	wstring m_wstrPosition = L"";

public:
	static CAnimationController* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CComponent* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END