#ifndef Aberrant_Flinch_Left_h__
#define Aberrant_Flinch_Left_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CAberrant_Flinch_Left final : public CMonster_FSM
{

protected:
	explicit CAberrant_Flinch_Left(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CAberrant_Flinch_Left(const CAberrant_Flinch_Left& rhs);
	virtual ~CAberrant_Flinch_Left() = default;

public:
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual _int Tick(const _double & TimeDelta);
	virtual _int LateTick(const _double & TimeDelta);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual void Look_Player(void) override;

public:
	static CAberrant_Flinch_Left* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Aberrant_Flinch_Left_h__
