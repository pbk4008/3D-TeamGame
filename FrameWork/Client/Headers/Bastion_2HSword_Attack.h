#ifndef Bastion_2HSword_Attack_h__
#define Bastion_2HSword_Attack_h__


#include "Bastion_2HSword_State.h"

BEGIN(Client)
class CBastion_2HSword_Attack : public CBastion_2HSword_State
{
protected:
	explicit CBastion_2HSword_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBastion_2HSword_Attack(const CBastion_2HSword_Attack& _rhs);
	virtual ~CBastion_2HSword_Attack() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();
	virtual HRESULT EnterState(void* pArg);
	virtual HRESULT ExitState(void* pArg);
private:
	void Play_Sound();
public:
	virtual void Look_Player(void) override;
	virtual void Look_Monster(void) override;

public:
	static CBastion_2HSword_Attack* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
private:
	_uint m_iType = 0;
};
END

#endif // Bastion_2HSword_Attack_h__