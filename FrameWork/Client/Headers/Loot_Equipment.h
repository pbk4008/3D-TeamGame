#ifndef Loot_Equipment_h__
#define Loot_Equipment_h__

#include "Hud.h"
#include "ItemData.h"

BEGIN(Client)
class CUI_LootBackground;
class CUI_LootFrame;
class CUI_LootGrade;
class CUI_LootItemIcon;
class CUI_LootItemEffect;
class CUI_LootItemNameText;
class CUI_LootItemNameType;

class CLoot_Equipment : public CHud
{
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
	//virtual void setActive(_bool bActive) override;

public:
	virtual HRESULT Ready_Component(void) override;
	virtual HRESULT Ready_UIObject(void) override;
	void	FadeIn(_double TimeDela);
	void	Pulling(_double TimeDelta);
	void	setPulling(_bool bSet) { m_bPulling = bSet; }
	void	SetActiveAll(_bool _OnOff);
public:
	_bool	GetAlive(void) { return m_bAlive; }
	void	SetGrade(CItemData itemData);
	void	SetFadeOutAll(void);

private:
	CUI_LootBackground*		m_pBG = nullptr;
	CUI_LootFrame*			m_pFrame = nullptr;
	CUI_LootGrade*			m_pGrade = nullptr;
	CUI_LootItemIcon*		m_pItemIcon = nullptr;
	CUI_LootItemEffect*		m_pItemEffect = nullptr;
	CUI_LootItemNameText*	m_pNameText = nullptr;
	CUI_LootItemNameType*	m_pNameType = nullptr;

private:
	CHud::Desc			desc;
	_float				m_fAliveTime = 0.f;
	_bool				m_bAlive = true;
	_bool				m_bPulling = false;
	_double				m_dTimedelta = 0.f;

	_float2				m_fInitScale;
	_float2				m_fEndScale;

	_float4				m_fInitPos = { 500.f, 80.f, 0.1f, 1.0f };
	_float4				m_fEndPos  = { 500.f, 130.f, 0.1f, 1.0f };

	_float				m_fDisaperXInitPos = 500.f;
	_float				m_fDisaperXEndPos = 530.f;

	_bool				m_bBye = false;
	_bool				m_bByeBye = false;

public:
	static CLoot_Equipment* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg) override;

private:
	virtual void Free() override;
};
END
#endif // Loot_Equipment_h__
