#pragma once
#ifndef UI_Indexes_h__
#define UI_Indexes_h__

#include "UI.h"

BEGIN(Client)

class CUI_Indexes : public CUI
{
public:
	explicit CUI_Indexes(void) = default;
	explicit CUI_Indexes(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_Indexes(const CUI_Indexes& rhs);
	virtual ~CUI_Indexes() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg);
	virtual _int Tick(_double dDeltaTime);

public:
	void Show(void);
	void Hide(void);
	void MenuAllOff(void);
	void Click_Equipment(void);
	void Click_Armory(void);

public:
	void SetMother(CGameObject* _pObj);
	void SetButton_Equipment(CGameObject* _pObj);
	void SetButton_Armory(CGameObject* _pObj);

private:
	CGameObject* m_pIndexMother = nullptr;
	CGameObject* m_pEquipmentBtn;
	CGameObject* m_pArmoryBtn;

private:
	_bool m_arrCurWindow[INVEN_INDEX::TAB_END] = { false };

public:
	static CUI_Indexes* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_Indexes_h__