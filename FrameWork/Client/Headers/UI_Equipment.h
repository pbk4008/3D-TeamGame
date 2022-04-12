#ifndef UI_Equipment_h__
#define UI_Equipment_h__

#include "UI.h"

BEGIN(Client)
class CInventoryData;
class CEquipmentData;
class CUI_ItemSlot;
class CUI_Equipment : public CUI
{
public:
	explicit CUI_Equipment(void) = default;
	explicit CUI_Equipment(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_Equipment(const CUI_Equipment& rhs);
	virtual ~CUI_Equipment() = default;

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
	void Show(void);
	void Hide(void);
	void UpdateSlots(void);
	void UpdateSlot(_int _iIndex);
	void UpdateResourceCount(void);
	void ClickSlot(void);
	void MouseOnSlot(void);

public: /* Get */
	_bool GetEquipmentActive(void);

public: /* Set */
	void InsertSlotVector(CUI_ItemSlot* _pObj);
	void SetResourceCount();

private:
	CInventoryData*	m_pInventoryData = nullptr;
	CEquipmentData* m_pEquipData	 = nullptr;

public:
	static CUI_Equipment* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	vector<CUI_ItemSlot*> m_vecSlots;
	_bool m_bEquipmentActive = false;

private:
	virtual void Free() override;
};

END
#endif // UI_Equipment_h__