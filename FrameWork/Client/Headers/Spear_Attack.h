#ifndef Spear_Attack_h__
#define Spear_Attack_h__

#include "Spear_State.h"

BEGIN(Client)
class CSpear_Attack : public CSpear_State
{
protected:
	explicit CSpear_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSpear_Attack(const CSpear_Attack& _rhs);
	virtual ~CSpear_Attack() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();
	virtual HRESULT EnterState(void* pArg);
	virtual HRESULT ExitState(void* pArg);

public:
	virtual void Look_Player(void) override;
	virtual void Look_Monster(void) override;

private:
	void Play_Sound(void);
	void Check_Attack();

private:
	_uint m_iAttack1Frame = 154;
	_uint m_iAttack2Frame = 220;
	_bool m_bAttack1 = false;
	_bool m_bAttack2 = false;

public:
	static CSpear_Attack* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};
END

#endif // Spear_Attack_h__
