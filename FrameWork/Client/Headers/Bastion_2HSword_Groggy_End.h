#ifndef Bastion_2HSword_Groggy_End_h__
#define Bastion_2HSword_Groggy_End_h__

#include "Bastion_2HSword_State.h"

BEGIN(Client)

class CBastion_2HSword_Groggy_End final : public CBastion_2HSword_State
{
private:
	explicit CBastion_2HSword_Groggy_End(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CBastion_2HSword_Groggy_End() = default;

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

public:
	static CBastion_2HSword_Groggy_End* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
private:
	virtual void Free() override;
};
END

#endif // Bastion_2HSword_Groggy_End_h__
