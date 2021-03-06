#ifndef UI_ItemSlot_h__
#define UI_ItemSlot_h__

#include "UI.h"

BEGIN(Client)
class CUI_SlotBackground;
class CUI_SlotItemIcon;
class CUI_SlotGrade;
class CUI_SlotItemEffect;
class CUI_EquipedText;
class CUI_SelectEffect;

class CUI_ItemSlot : public CUI
{
public:
	struct ItemSlotDesc
	{
		_float2 fPos;
		_float2 fScale;
		CUI*	pOwner = nullptr;
	};

	explicit CUI_ItemSlot(void) = default;
	explicit CUI_ItemSlot(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_ItemSlot(const CUI_ItemSlot& rhs);
	virtual ~CUI_ItemSlot() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual void setActive(_bool bActive) override;

public:
	HRESULT Ready_Component(void);
	HRESULT Ready_UIObject(void);

public:
	void	SetActiveAll(_bool _OnOff);
	void	SetActiveExceptBg(_bool _OnOff);
	void	SetActiveOnlyBg(_bool _OnOff);
	void	SetActiveOnlnyIcon(_bool _OnOff);
	void	SetActiveEquiped(_bool _IsEquiped);

	_bool	BgMouseOn(void);
	_bool	BgClicked(_int _KeyState);
	_bool	IconMouseOn(void);
	_bool	IconClicked(void);
	_bool	ItemClicked(void);

	void	SetIcon(const std::wstring& _szFileName);
	void    SetGrade(EEquipmentGrade _eGrade);

private:
	CUI_SlotBackground*		m_pBG = nullptr;
	CUI_SlotItemIcon*		m_pItemIcon = nullptr;
	CUI_SlotGrade*			m_pGrade = nullptr;
	CUI_SlotItemEffect*		m_pItemEffect = nullptr;
	CUI_EquipedText*		m_pEquipedText = nullptr;
	CUI_SelectEffect*		m_pSelectEffect = nullptr;

	ItemSlotDesc desc;

private:
	_float m_fInitPos = 10.f;
	_float m_fEndPos = 0.f;

public:
	static CUI_ItemSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};
END
#endif // UI_ItemSlot_h__

