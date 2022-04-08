#ifndef Spear_Charge_Attack_h__
#define Spear_Charge_Attack_h__

#include "Spear_State.h"

BEGIN(Client)
class CSpear_Charge_Attack : public CSpear_State
{
protected:
	explicit CSpear_Charge_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSpear_Charge_Attack(const CSpear_Charge_Attack& _rhs);
	virtual ~CSpear_Charge_Attack() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();
public:
	virtual void Look_Player(void) override;
	virtual void Look_Monster(void) override;

private:
	void Play_Sound();
	void Check_Attack();
public:
	static CSpear_Charge_Attack* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};
END

#endif // Spear_Charge_Attack_h__


