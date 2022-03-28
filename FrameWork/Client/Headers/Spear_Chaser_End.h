#ifndef Spear_Chaser_End_h__
#define Spear_Chaser_End_h__

#include "Spear_State.h"

BEGIN(Client)
class CSpear_Chaser_End final : public CSpear_State
{

protected:
	explicit CSpear_Chaser_End(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSpear_Chaser_End(const CSpear_Chaser_End& _rhs);
	virtual ~CSpear_Chaser_End() = default;

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
	_bool	m_bStopChaser = false;

public:
	static CSpear_Chaser_End* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Spear_Chaser_End_h__
