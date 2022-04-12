#ifndef UI_SlotItemIcon_h__
#define UI_SlotItemIcon_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUIHelper;
class CUI_SlotItemIcon : public CUI
{
public:
	struct ItemSlotDesc
	{
		_float2 fPos;
		_float2 fScale;
	};

public:
	explicit CUI_SlotItemIcon(void) = default;
	explicit CUI_SlotItemIcon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_SlotItemIcon(const CUI_SlotItemIcon& rhs);
	virtual ~CUI_SlotItemIcon() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);

public:
	_bool	IconMouseOn(void);
	_bool	IconClicked(void);
	_bool   ItemClicked(void);
	void	SetIcon(const std::wstring& _szFileName);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CUIHelper*	  m_pUIHelperCom = nullptr;
	ItemSlotDesc  desc;
	_bool		  m_bHasItem = false;
public:
	static CUI_SlotItemIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_SlotItemIcon_h__
