#pragma once

#include "State.h"
#include "Silvermane.h"
#include "Weapon.h"

BEGIN(Client)

class CSilvermane;

class CState_Silvermane abstract : public CState
{
public:
	enum class EDir { Left, Forward, Right, Backward, LeftForward, RightForward, LeftBackward, RightBackward, Max};
protected:
	explicit CState_Silvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CState_Silvermane() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	void Set_Silvermane(CSilvermane* _pSilvermane);
	void Set_Transform(CTransform* _pTransform);
	void Set_Model(CModel * _pModel);
	void Set_AnimationController(CAnimationController* _pAnimationController);

protected:
	const _int Add_PlusAngle(const EDir _eDir, const _double& _dDeltaTime);

protected:
	virtual _int KeyCheck(const _double& _dDeltaTime);

protected:
	CSilvermane* m_pSilvermane = nullptr;
	CTransform* m_pTransform = nullptr;
	CModel* m_pModel = nullptr;
	CAnimationController* m_pAnimationController = nullptr;
	_uint m_iCutIndex = 0;
	EDir m_eDir = EDir::Max;

public:
	virtual void Free() override;
};

END