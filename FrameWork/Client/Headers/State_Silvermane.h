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
	const _int ToIdle();
	const _int ToSprint();
	const _int ToJogAttack();
	const _int ToAttack();
	const _int ToDashAttack();
	const _int ToChargeStart();
	const _int ToJogFwd();
	const _int ToJogBwd();
	const _int ToJogLeft();
	const _int ToJogRight();

protected:
	virtual _int Input(const _double& _dDeltaTime);
	void Reflect_Bullet(const ATTACKDESC& _tAttackDesc);

public:
	virtual void Hit(const ATTACKDESC& _tAttackDesc);
	virtual void Block(const ATTACKDESC& _tAttackDesc);
	virtual void Death(const ATTACKDESC& _tAttackDesc);

protected:
	CSilvermane* m_pSilvermane = nullptr;
	CTransform* m_pTransform = nullptr;
	CModel* m_pModel = nullptr;
	CAnimationController* m_pAnimationController = nullptr;

	_bool m_isShake = false;
	_uint m_iCutIndex = 0;
	EDir m_eDir = EDir::Max;

	

public:
	virtual void Free() override;
};

END