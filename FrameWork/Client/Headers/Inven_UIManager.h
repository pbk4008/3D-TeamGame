#ifndef Inven_UIManager_h__
#define Inven_UIManager_h__

#include "SingleTon.h"
BEGIN(Client)
class CUI_Indexes;
class CUI_ModalWindow;
class CUI_ItemStatusWindow;
class CUI_PlayerStatusWindow;
class CHud;
class CItemData;
class CInven_UIManager final : public CSingleTon<CInven_UIManager>
{
	friend CSingleTon;

private:
	NO_COPY(CInven_UIManager);
	CInven_UIManager(void);
	~CInven_UIManager(void) = default;

public:
	HRESULT NativeConstruct(void);
	_int    Tick(_double _dTimeDelta);

public:
	void OnModalOpen(void*);
	void OnModalClose(void*);

public: /* Open Modal */
	void  OpenModal(void);
	void  CloseModal(void);
	_bool IsOpenModal(void);
	void ShowLevelUp(void);
public: 
	_bool Get_OpenCheck(void) { return m_bOpenModal; }
	CHud* GetHud(void);

public: /* Item */
	void ShowItemStatus(CItemData* _pItemData);
	void HideItemStatus(void);

private: /* Inventory */
	CUI_ModalWindow*			m_pModal = nullptr;
	CUI_ItemStatusWindow*		m_pItemStatus = nullptr;
	CUI_PlayerStatusWindow*		m_pPlayerStatus = nullptr;

private: /* Notify UI */
	CHud*	m_pHud = nullptr;

public: /* set Level tex */
	void	SetLevelBG(_int PlayerLevel);

public:
	void SetRender(_bool bOnoff) { m_bRender = bOnoff; }

private:
	_bool m_bOpenModal = false;
	_bool m_bRender = true;

private:
	virtual void Free() override;
};

END
#endif // Inven_UIManager_h__
