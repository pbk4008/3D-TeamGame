#ifndef Hud_h__
#define Hud_h__

#include "UI.h"
#include "ItemData.h"

BEGIN(Client)
class CLoot_Equipment;
class CLevel_UP;
class CPlayerData;
class CEquipmentData;
class CUI_EquippedWeapon;
class CUI_EquippedWeapon_Slot_1;
class CUI_EquippedWeapon_Slot_2;
class CMapInfo;

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
	const std::wstring arrMapInfo[2]
	{
		L"T_HUD_Stage_1",
		L"T_HUD_MidBoss",
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
	void ShowMapInfo_HUD(_bool bOnOff, _int TexIdx);
	void ShowLevelUp_HUD(_int iLevel);

public:
	void OnLootEquipment(void* pItemData);
	void CheckEuipWeaponSlot(void);
	void CheckCurEquipSlot(void);
	void SetActiveOnlyDefault(void);
	void SetActiveOnlySlots(void);

public:
	void SetLevelBG(_int PlayerLevel);
	void FixPos(void);
	void EquipmentRenderNo(_bool bOnoff);
	void EquipmentRenderYes(void);

private:
	CLevel_UP*				   m_pLevelUp = nullptr;
	CUI_EquippedWeapon*		   m_pEquipWeapon = nullptr;
	CUI_EquippedWeapon_Slot_1* m_pEquipWeapon_Slot_1 = nullptr;
	CUI_EquippedWeapon_Slot_2* m_pEquipWeapon_Slot_2 = nullptr;
	CMapInfo*				   m_pMapInfo = nullptr;

private:
	CPlayerData*	m_pPlayerData = nullptr;
	CEquipmentData* m_pEquipData = nullptr;

private:
	std::vector< CLoot_Equipment*> m_vecLootEquipment;
	
private:
	_bool  m_bOnLevelUpUI = false;
	_bool  m_bHideLevelUpUI = false;
	_bool  m_bShowMapInfo_HUD = false;
	_float m_fOnTime = 0.f;
	_float m_fMapInfoTime = 0.f;
	_bool  m_bRender = true;

public:
	static CHud* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr);
	virtual void Free() override;
};

END
#endif // Hud_h__
