#ifndef BronzeAnimus_Chaser_h__
#define BronzeAnimus_Chaser_h__

#include "BronzeAnimus_State.h"

BEGIN(Client)
class CBronzeAnimus_Chaser final : public CBronzeAnimus_State
{

protected:
	explicit CBronzeAnimus_Chaser(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBronzeAnimus_Chaser(const CBronzeAnimus_Chaser& _rhs);
	virtual ~CBronzeAnimus_Chaser() = default;

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
	static CBronzeAnimus_Chaser* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END

#endif // BronzeAnimus_Chaser_h__
