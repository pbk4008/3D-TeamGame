#ifndef Bastion_2HSword_Dash_h__
#define Bastion_2HSword_Dash_h__

#include "Monster_FSM.h"

BEGIN(Client)
class CBastion_2HSword_Dash final : public CMonster_FSM
{
protected:
	explicit CBastion_2HSword_Dash(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBastion_2HSword_Dash(const CBastion_2HSword_Dash& _rhs);
	virtual ~CBastion_2HSword_Dash() = default;

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
	static CBastion_2HSword_Dash* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};
END
#endif // Bastion_2HSword_Dash_h__