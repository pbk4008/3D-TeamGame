#ifndef Inven_UIManager_h__
#define Inven_UIManager_h__

#include "SingleTon.h"
BEGIN(Client)
class CUI_Indexes;
class CUI_ModalWindow;
class CUI_ItemStatusWindow;
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
	_int    Tick(void);

public:
	void OnModalOpen(void*);
	void OnModalClose(void*);

public: /* Open Modal */
	void  OpenModal(void);
	void  CloseModal(void);
	_bool IsOpenModal(void);

public: _bool Get_OpenCheck() { return m_bOpenModal; }

public: /* Item */
	void ShowItemStatus(CItemData* _pItemData);
	void HideItemStatus(void);

private: /* Inventory */
	CUI_ModalWindow*			m_pModal = nullptr;
	CUI_ItemStatusWindow*		m_pItemStatus = nullptr;

private:
	_bool m_bOpenModal = false;

private:
	virtual void Free() override;
};

END
#endif // Inven_UIManager_h__
