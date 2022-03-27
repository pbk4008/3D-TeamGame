#ifndef BronzeAnimus_Hit_h__
#define BronzeAnimus_Hit_h__

#include "BronzeAnimus_State.h"

BEGIN(Client)
class CBronzeAnimus_Hit final : public CBronzeAnimus_State
{
protected:
	explicit CBronzeAnimus_Hit(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBronzeAnimus_Hit(const CBronzeAnimus_Hit& _rhs);
	virtual ~CBronzeAnimus_Hit() = default;

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

public:
	static CBronzeAnimus_Hit* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};
END

#endif // BronzeAnimus_Hit_h__
