#ifndef UI_PlayerStatusWindow_h__
#define UI_PlayerStatusWindow_h__

#include "UI.h"

BEGIN(Client)
class CEquipmentData;
class CUI_PlayerStatus_BG;
class CUI_PlayerStatus_Might;
class CUI_PlayerStatus_Spirit;
class CUI_PlayerStatus_Viality;
class CUI_PlayerStatus_DPS_1;
class CUI_PlayerStatus_DPS_2;
class CUI_PlayerStatus_MAXHP;

class CUI_PlayerStatusWindow : public CUI
{
public:
	struct Desc
	{
		_float2 fPos;
		_float2 fScale;
		CUI*	pOwner = nullptr;
	};

	explicit CUI_PlayerStatusWindow(void) = default;
	explicit CUI_PlayerStatusWindow(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_PlayerStatusWindow(const CUI_PlayerStatusWindow& rhs);
	virtual ~CUI_PlayerStatusWindow() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	HRESULT Ready_UIObject(void);

public:
	void	UpdateStatus(void);
	void    UpdateDPS(void);
	void    UpdateMaxHP(void);
	void	SetActiveAll(_bool _OnOff);
	void	Show(void);
	void	Hide(void);

private:
	CUI_PlayerStatus_BG*	  m_pBG     = nullptr;
	CUI_PlayerStatus_Might*   m_pMight  = nullptr;    /* 근력 */
	CUI_PlayerStatus_Spirit*  m_pSpirit = nullptr;    /* 정신력 */
	CUI_PlayerStatus_Viality* m_pViality = nullptr;   /* 활력 */
	CUI_PlayerStatus_DPS_1*	  m_pFirstDPS = nullptr;  /* 주무장 DPS */
	CUI_PlayerStatus_DPS_2*	  m_pSecondDPS = nullptr; /* 부무장 DPS */
	CUI_PlayerStatus_MAXHP*	  m_pMaxHp = nullptr;     /* 최대체력 */
	CEquipmentData*			  m_pEquipData = nullptr;

	Desc desc;
public:
	static CUI_PlayerStatusWindow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};
END
#endif // UI_PlayerStatusWindow_h__
