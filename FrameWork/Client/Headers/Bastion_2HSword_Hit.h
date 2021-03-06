#ifndef Bastion_2HSword_Hit_h__
#define Bastion_2HSword_Hit_h__

#include "Bastion_2HSword_State.h"

BEGIN(Client)
class CBastion_2HSword_Hit final : public CBastion_2HSword_State
{
protected:
	explicit CBastion_2HSword_Hit(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBastion_2HSword_Hit(const CBastion_2HSword_Hit& _rhs);
	virtual ~CBastion_2HSword_Hit() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT EnterState(void* _pArg = nullptr);
	virtual HRESULT ExitState();

public:
	virtual void Look_Player(void) override;
	virtual void Look_Monster(void) override;

private:
	_float3 m_vDir{};

public:
	static CBastion_2HSword_Hit* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};
END

#endif // Bastion_2HSword_Hit_h__