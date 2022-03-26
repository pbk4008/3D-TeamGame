#ifndef BronzeAnimus_Battle_Idle_h__
#define BronzeAnimus_Battle_Idle_h__

#include "BronzeAnimus_State.h"

BEGIN(Client)
class CBronzeAnimus_Battle_Idle final : public CBronzeAnimus_State
{
protected:
	explicit CBronzeAnimus_Battle_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBronzeAnimus_Battle_Idle(const CBronzeAnimus_Battle_Idle& _rhs);
	virtual ~CBronzeAnimus_Battle_Idle() = default;

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
	_bool	m_bBattleOn = false;

public:
	static CBronzeAnimus_Battle_Idle* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END

#endif // BronzeAnimus_Battle_Idle_h__
