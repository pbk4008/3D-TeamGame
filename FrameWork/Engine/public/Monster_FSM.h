#ifndef Monster_FSM_h__
#define Monster_FSM_h__

#include "State.h"

BEGIN(Engine)
class CAnimator;
class CModel;
class CActor;
class CTransform;
class CStateController;

class ENGINE_DLL CMonster_FSM abstract : public CState
{
public:
	typedef struct tagStateFSMDesc
	{
		CAnimator* pAnimator;
		CStateController* pController;
	}FSMDESC;
	typedef struct tagStateFSMMoveDesc : public tagStateFSMDesc
	{
		CTransform* pTransform;
	}FSMMOVEDESC;
protected:
	explicit CMonster_FSM(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CMonster_FSM() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	void Set_Monster(CActor* _pMonster)				 { m_pMonster = _pMonster; }
	void Set_Transform(CTransform* _pTransform)		 { m_pTransform = _pTransform; }
	void Set_Model(CModel* _pModel)				     { m_pModel = _pModel; }
	void Set_Animator(CAnimator* _pAnimator)		 { m_pAnimator = _pAnimator; }

protected:
	virtual void Look_Player(void); /* 플레이어 상태 추적 */
	virtual void Look_Monster(void); /* 몬스터 자신의 상태 추적 */

protected:
	CAnimator*  m_pAnimator  = nullptr;
	CActor*		m_pMonster   = nullptr;
	CTransform* m_pTransform = nullptr;
	CModel*		m_pModel     = nullptr;

public:
	virtual void Free() override;
};

END
#endif // Monster_FSM_h__
