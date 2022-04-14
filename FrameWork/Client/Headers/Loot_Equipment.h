#ifndef Loot_Equipment_h__
#define Loot_Equipment_h__

#include "Hud.h"
#include "ItemData.h"

BEGIN(Client)

class CLoot_Equipment : public CHud
{
public:
	struct Desc
	{
		CItemData itemData;
		_float	  fDisapeatTime = 5.f;
		_int	  iQueueIndex = 0;
	};
public:
	explicit CLoot_Equipment(void) = default;
	explicit CLoot_Equipment(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CLoot_Equipment(const CHud & rhs);
	virtual ~CLoot_Equipment() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual void setActive(_bool bActive) override;

public:
	virtual HRESULT Ready_Component(void) override;
	virtual HRESULT Ready_UIObject(void) override;

public:
	static CLoot_Equipment* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg) override;

private:
	virtual void Free() override;
};
END
#endif // Loot_Equipment_h__
