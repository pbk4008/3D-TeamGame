#ifndef Bastion_2HSword_Idle_h__
#define Bastion_2HSword_Idle_h__

#include "Monster_FSM.h"

BEGIN(Client)
class CBastion_2HSword_Idle final : public CMonster_FSM
{
protected:
	explicit CBastion_2HSword_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBastion_2HSword_Idle(const CBastion_2HSword_Idle& _rhs);
	virtual ~CBastion_2HSword_Idle() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual void Look_Player(void) override;

public:
	static CBastion_2HSword_Idle* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END


#endif // Bastion_2HSword_Idle_h__