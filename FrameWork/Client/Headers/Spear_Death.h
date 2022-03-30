#ifndef Spear_Death_h__
#define Spear_Death_h__
#include "Spear_State.h"

BEGIN(Client)
class CSpear_Death final : public CSpear_State
{
protected:
	explicit CSpear_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSpear_Death(const CSpear_Death& _rhs);
	virtual ~CSpear_Death() = default;

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
	_bool	m_bDeath = false;

public:
	static CSpear_Death* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};
END
#endif // Spear_Death_h__