#ifndef Spear_Chaser_h__
#define Spear_Chaser_h__

#include "Spear_State.h"

BEGIN(Client)
class CSpear_Chaser final : public CSpear_State
{

protected:
	explicit CSpear_Chaser(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSpear_Chaser(const CSpear_Chaser& _rhs);
	virtual ~CSpear_Chaser() = default;

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
	static CSpear_Chaser* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END
#endif // Spear_Chaser_h__

