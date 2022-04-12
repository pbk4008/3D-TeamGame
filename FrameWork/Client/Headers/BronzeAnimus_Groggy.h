#ifndef BronzeAnimus_Groggy_h__
#define BronzeAnimus_Groggy_h__

#include "BronzeAnimus_State.h"

BEGIN(Client)

class CBronzeAnimus_Groggy final : public CBronzeAnimus_State
{
private:
	explicit CBronzeAnimus_Groggy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CBronzeAnimus_Groggy() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual void Look_Player(void) override;
	virtual void Look_Monster(void) override;
	virtual void OnTriggerEnter(CCollision& collision) override;

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

private:
	_float m_fGroggyTime = 5.0f;
public:
	static CBronzeAnimus_Groggy* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
private:
	virtual void Free() override;
};
END

#endif // BronzeAnimus_Groggy_h__
