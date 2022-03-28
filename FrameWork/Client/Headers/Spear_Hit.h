#ifndef Spear_Hit_h__
#define Spear_Hit_h__

#include "Spear_State.h"

BEGIN(Client)
class CSpear_Hit final : public CSpear_State
{
protected:
	explicit CSpear_Hit(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSpear_Hit(const CSpear_Hit& _rhs);
	virtual ~CSpear_Hit() = default;

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

public:
	static CSpear_Hit* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};
END

#endif // Spear_Hit_h__