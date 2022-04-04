#ifndef Bastion_Healer_Chaser_h__
#define Bastion_Healer_Chaser_h__

#include "Bastion_Healer_State.h"

BEGIN(Client)
class CBastion_Healer_Run final : public CBastion_Healer_State
{

protected:
	explicit CBastion_Healer_Run(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBastion_Healer_Run(const CBastion_Healer_Run& _rhs);
	virtual ~CBastion_Healer_Run() = default;

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
	static CBastion_Healer_Run* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Bastion_Healer_Chaser_h__
