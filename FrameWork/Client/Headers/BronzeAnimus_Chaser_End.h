#ifndef BronzeAnimus_Chaser_End_h__
#define BronzeAnimus_Chaser_End_h__

#include "BronzeAnimus_State.h"

BEGIN(Client)
class CBronzeAnimus_Chaser_End final : public CBronzeAnimus_State
{

protected:
	explicit CBronzeAnimus_Chaser_End(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBronzeAnimus_Chaser_End(const CBronzeAnimus_Chaser_End& _rhs);
	virtual ~CBronzeAnimus_Chaser_End() = default;

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
	static CBronzeAnimus_Chaser_End* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END

#endif // BronzeAnimus_Chaser_End_h__

