#ifndef BronzeAnimus_Roar_h__
#define BronzeAnimus_Roar_h__

#include "BronzeAnimus_State.h"

BEGIN(Client)
class CBronzeAnimus_Roar final : public CBronzeAnimus_State
{
protected:
	explicit CBronzeAnimus_Roar(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBronzeAnimus_Roar(const CBronzeAnimus_Roar& _rhs);
	virtual ~CBronzeAnimus_Roar() = default;

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
	static CBronzeAnimus_Roar* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};
END

#endif // BronzeAnimus_Roar_h__