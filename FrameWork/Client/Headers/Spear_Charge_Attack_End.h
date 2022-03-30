#ifndef Spear_Charge_Attack_End_h__
#define Spear_Charge_Attack_End_h__

#include "Spear_State.h"

BEGIN(Client)
class CSpear_Charge_Attack_End : public CSpear_State
{
protected:
	explicit CSpear_Charge_Attack_End(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSpear_Charge_Attack_End(const CSpear_Charge_Attack_End& _rhs);
	virtual ~CSpear_Charge_Attack_End() = default;

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
	_float m_fTime = 5.0f;

public:
	static CSpear_Charge_Attack_End* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};
END

#endif // Spear_Charge_Attack_End_h__