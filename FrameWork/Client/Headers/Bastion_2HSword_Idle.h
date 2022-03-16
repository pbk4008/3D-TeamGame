#ifndef Bastion_2HSword_Idle_h__
#define Bastion_2HSword_Idle_h__

#include "State_Monster.h"

BEGIN(Client)

class CBastion_2HSword_Idle final : public CState_Monster
{
protected:
	explicit CBastion_2HSword_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBastion_2HSword_Idle(const CBastion_2HSword_Idle& _rhs);
	virtual ~CBastion_2HSword_Idle() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int KeyCheck(const _double & _dDeltaTime) override;

private:
	_float m_fHoldTime = 0.f;

public:
	static CBastion_2HSword_Idle* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END


#endif // Bastion_2HSword_Idle_h__