#ifndef BronzeAnimus_Death_h__
#define BronzeAnimus_Death_h__

#include "BronzeAnimus_State.h"

BEGIN(Client)
class CBronzeAnimus_Death final : public CBronzeAnimus_State
{
protected:
	explicit CBronzeAnimus_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBronzeAnimus_Death(const CBronzeAnimus_Death& _rhs);
	virtual ~CBronzeAnimus_Death() = default;

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
	CCameraShake::SHAKEEVENT m_tShakeEvent;

public:
	static CBronzeAnimus_Death* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};
END

#endif // BronzeAnimus_Death_h__