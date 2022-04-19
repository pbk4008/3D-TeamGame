#ifndef Hud_h__
#define Hud_h__

#include "UI.h"
#include "ItemData.h"

BEGIN(Client)
class CLoot_Equipment;
class CLevel_UP;
class CPlayerData;
class CHud : public CUI
{
public:
	struct Desc
	{
		CItemData itemData;
		_float	  fDisapeatTime = 5.f;
		_int	  iQueueIndex = 0;
		CHud*	  pOwner = nullptr;
		_float2   fInitPos = { 0.f, 0.f };
		_float2   fInitScale = { 1.2f, 1.2f };
		_float2   fEndScale = { 1.0f, 1.0f };
	};

public:
	explicit CHud(void) = default;
	explicit CHud(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CHud(const CHud& rhs);
	virtual ~CHud() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	//virtual void setActive(_bool bActive) override;

public:
	virtual HRESULT Ready_Component(void);
	virtual HRESULT Ready_UIObject(void);

public:
	void PullingEquipmentUI(void);
	void ShowLevelUp_HUD(void);

public:
	void OnLootEquipment(void* pItemData);

private:
	CLevel_UP* m_pLevelUp = nullptr;
	CPlayerData* m_pPlayerData = nullptr;
private:
	std::vector< CLoot_Equipment*> m_vecLootEquipment;
public:
	static CHud* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr);
	virtual void Free() override;
};

END
#endif // Hud_h__
