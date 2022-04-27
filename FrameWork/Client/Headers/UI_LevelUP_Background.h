#ifndef UI_LevelUP_Background_h__
#define UI_LevelUP_Background_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_LevelUP_Background : public CUI
{
public:
	struct ItemSlotDesc
	{
		_float2 fPos;
		_float2 fScale;
		CUI*	pOwner = nullptr;
	};

	struct Desc
	{
		CItemData itemData;
		_float	  fDisapeatTime = 5.f;
		_int	  iQueueIndex = 0;
		CUI*	  pOwner = nullptr;
		_float2   fInitPos = { 0.f, 0.f };
		_float2   fInitScale = { 1.2f, 1.2f };
		_float2   fEndScale = { 1.0f, 1.0f };
	};

	explicit CUI_LevelUP_Background(void) = default;
	explicit CUI_LevelUP_Background(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_LevelUP_Background(const CUI& rhs);
	virtual ~CUI_LevelUP_Background() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	_int Attach_Owner(void);

public:
	void Show(_double dTimeDelta);
	void SetBg(_int PlayerLevel);
	void SetFadeOut(void);
	void FadeIn(void);
	void FixPos(void);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CTransform*   m_pLocalTransform = nullptr;
	CUI*		  m_pOwner = nullptr;
	Desc		  desc;
	_bool		  m_bShow = false;
	
private:
	_float2		  m_fInitScale = { 80.f, 80.f };
	_float2		  m_fEndScale = { 47.f , 47.f };

private:
	enum class ELevel
	{
		Level_1,
		Level_2,
		Level_3,
		Level_4,
		Level_5,
		Level_6,
		Level_7,
		Level_8,
		Level_9,
		Level_End
	};

	const wstring m_arrLevelBgTex[(int)ELevel::Level_End]
	{
		L"T_HUD_Level_BG_1",
		L"T_HUD_Level_BG_2",
		L"T_HUD_Level_BG_3",
		L"T_HUD_Level_BG_4",
		L"T_HUD_Level_BG_5",
		L"T_HUD_Level_BG_6",
		L"T_HUD_Level_BG_7",
		L"T_HUD_Level_BG_8",
		L"T_HUD_Level_BG_9",
	};

public:
	static CUI_LevelUP_Background* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_LevelUP_Background_h__
