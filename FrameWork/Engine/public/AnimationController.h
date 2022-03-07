#pragma once

#include "Component.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CModel;
class CAnimation;
class CChannel;

class ENGINE_DLL CAnimationController final : public CComponent
{
public:
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
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	const _uint Get_CurAnimIndex() const;
	const _uint Get_CurKeyFrameIndex() const;
	const _uint Get_CurFixedBoneKeyFrameIndex() const;
	const _uint Get_MaxKeyFrameIndex() const;
	const ERootOption Get_RootOption() const;

	void Set_GameObject(CGameObject* _pGameObject);
	void Set_Transform(CTransform* _pTransform);
	void Set_Model(CModel* _pModel);
	void Set_LoopAnim(const _bool _isLoop = true);
	void Set_TransformMove(const _bool _isTransformMove);
	void Set_PivotMatrix(const _fmatrix& _smatPivot);
	void Set_RootMotion(const _bool _isRootMotion, const _bool _isTransformMove, const ERootOption _eRootOption = ERootOption::XYZ);

	const _bool Is_RootMotion() const;
	const _bool Is_Finished() const;

public:
	_int Update_CombinedTransformMatrix(const _double& _dDeltaTime);
	void Lerp_Anim(vector<CAnimation*>& _vecAnimations);
	HRESULT SetUp_NextAnimation(const string& _strAnimTag, const _bool _isLoopNextAnim = true);
	HRESULT Change_Anim(const string& _strAnimTag, _bool _isLoop = true);
	const _int Move_Transform(const _double& _dDeltaTime);

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

	_matrix m_smatPivot = XMMatrixIdentity();

	ANIMBLENDDESC m_tBlendDesc;

	// 디버그용
	wstring m_wstrFixedBonePosition = L"";
	wstring m_wstrPosition = L"";
	wstring m_wstrCurAnimTag = L"";
	wstring m_wstrPreIndex = L"";
	wstring m_wstrCurIndex = L"";
	wstring m_wstrIsFinished = L"";

public:
	static CAnimationController* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CComponent* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END