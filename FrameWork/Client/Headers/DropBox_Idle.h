#ifndef DropBox_Idle_h__
#define DropBox_Idle_h__

#include "DropBox_State.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)

class CDropBox_Idle final : public CDropBox_State
{
protected:
	explicit CDropBox_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CDropBox_Idle(const CDropBox_Idle& rhs);
	virtual ~CDropBox_Idle() = default;

public:
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual _int Tick(const _double & TimeDelta);
	virtual _int LateTick(const _double & TimeDelta);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	static CDropBox_Idle* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END
#endif // DropBox_Idle_h__