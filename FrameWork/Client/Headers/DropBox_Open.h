#ifndef DropBox_Open_h__
#define DropBox_Open_h__

#include "DropBox_State.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CDropBox_Open final : public CDropBox_State
{
protected:
	explicit CDropBox_Open(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CDropBox_Open(const CDropBox_Open& rhs);
	virtual ~CDropBox_Open() = default;

public:
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual _int Tick(const _double & TimeDelta);
	virtual _int LateTick(const _double & TimeDelta);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	static CDropBox_Open* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};
END

#endif // DropBox_Open_h__