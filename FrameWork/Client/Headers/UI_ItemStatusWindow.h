#ifndef IU_ItemStatusWindow_h__
#define IU_ItemStatusWindow_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class UI_Texture;
END

BEGIN(Client)
class CSingleImage;
class CItemData;
class UI_ItemStatusBackground;
class UI_ItemStatusEffect;
class CUI_ItemStatusWindow : public CUI
{
public:
	struct Desc
	{
		CSingleImage* pBgSprite = nullptr;
		UI_Texture*   pRequiredLvText = nullptr;
		UI_Texture*   ptemNameText = nullptr;
		UI_Texture*   pGradeAndKindText = nullptr;
		UI_Texture*   pMainStatText = nullptr;
		UI_Texture*	  pMainStatKind = nullptr;
	};

public:
	explicit CUI_ItemStatusWindow(void) = default;
	explicit CUI_ItemStatusWindow(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_ItemStatusWindow(const CUI_ItemStatusWindow& rhs);
	virtual ~CUI_ItemStatusWindow() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	HRESULT Ready_UIObject(void);
	void    ResetInitPos(void) { m_fInitPos = 10.f; }

public:
	void Show(CItemData* _pItem);
	void Hide();
	void SettingItemStatus();
	wstring GradeString(EEquipmentGrade _eGrade);
	wstring TypeString(EEquipmentType _eType);

public: /* Get */
	CGameObject* GetUI(const wstring& _szUIName);

public: /* Set */
	void InsertDefaultUIs(const wstring& _szName, CGameObject* _pObj);

private:
	CItemData* m_pSelectItem = nullptr;
	_bool	   m_bIsOpen = true;
	_float2	   m_vBgScale = { 485.f, 845.f };

private:
	CSingleImage*	 m_pBgSprite = nullptr;
	CSingleImage*	 m_pEffectSprite = nullptr;
	UI_Texture*		 m_pRequiredLvText = nullptr;
	UI_Texture*		 m_ptemNameText = nullptr;
	UI_Texture*		 m_pGradeAndKindText = nullptr;
	UI_Texture*		 m_pMainStatText = nullptr;
	UI_Texture*		 m_pMainStatKind = nullptr;

private:
	UI_ItemStatusBackground*	m_pBg = nullptr;
	UI_ItemStatusEffect*		m_pEffect = nullptr;

private:
	std::map<wstring, CGameObject*> m_mapDefaultUIs;

private:
	_float m_fInitPos = 10.f;
	_float m_fEndPos = 0.f;

public:
	static CUI_ItemStatusWindow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // IU_ItemStatusWindow_h__

