#ifndef Spear_Bwd_Dash_h__
#define Spear_Bwd_Dash_h__

#include "Spear_State.h"

BEGIN(Client)
class CSpear_Bwd_Dash final : public CSpear_State
{
protected:
	explicit CSpear_Bwd_Dash(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSpear_Bwd_Dash(const CSpear_Bwd_Dash& _rhs);
	virtual ~CSpear_Bwd_Dash() = default;

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
	static CSpear_Bwd_Dash* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Spear_Bwd_Dash_h__
