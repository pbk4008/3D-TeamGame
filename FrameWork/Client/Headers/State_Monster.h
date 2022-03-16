#ifndef State_Monster_h__
#define State_Monster_h__

#include "State.h"
#include "Silvermane.h"
#include "Weapon.h"

BEGIN(Engine)
class CActor;
END

BEGIN(Client)

class CState_Monster abstract : public CState
{
public:
	enum class EDir { Left, Forward, Right, Backward, LeftForward, RightForward, LeftBackward, RightBackward, Max};
protected:
	explicit CState_Monster(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CState_Monster() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render(void);

public:
	virtual HRESULT EnterState(void);
	virtual HRESULT ExitState(void);

public:
	void Set_Monster(CActor* _pMonster);
	void Set_Transform(CTransform* _pTransform);
	void Set_Model(CModel * _pModel);
	void Set_AnimationController(CAnimationController* _pAnimationController);

protected:
	const _int Add_PlusAngle(const EDir _eDir, const _double& _dDeltaTime);

protected:
	virtual _int KeyCheck(const _double& _dDeltaTime);

protected:
	CActor*				  m_pMonster = nullptr;
	CTransform*			  m_pTransform = nullptr;
	CModel*				  m_pModel = nullptr;
	CAnimationController* m_pAnimationController = nullptr;
	_uint				  m_iCutIndex = 0;
	EDir				  m_eDir = EDir::Max;

public:
	virtual void Free() override;
};

END


#endif // State_Monster_h__