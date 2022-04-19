#pragma once
#ifndef UI_Indexes_h__
#define UI_Indexes_h__

#include "UI.h"

BEGIN(Client)
class CButton_Equipment;
class CButton_Armory;
class CButton_Skill;
class CIndexIndicator;

class CUI_Indexes final : public CUI
{
public:
	explicit CUI_Indexes(void) = default;
	explicit CUI_Indexes(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_Indexes(const CUI_Indexes& rhs);
	virtual ~CUI_Indexes() = default;

public:
	virtual HRESULT NativeConstruct_Prototype()override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg)override;
	virtual _int Tick(_double dDeltaTime)override;
	virtual _int LateTick(_double TimeDelta)override;
	virtual HRESULT Render(void) override;
	virtual void setActive(_bool bActive) override;

public:
	HRESULT Ready_UIObject(void);

public:
	void Show(void);
	void Hide(void);
	void MenuAllOff(void);
	_bool Click_Equipment(void);
	_bool Click_Armory(void);
	_bool Click_Skill(void);

public:
	void SetMother(CGameObject* _pObj);

private:
	CGameObject*	   m_pIndexMother = nullptr;
	CButton_Equipment* m_pEquipmentBtn = nullptr;
	CButton_Armory*	   m_pArmoryBtn = nullptr;
	CButton_Skill*	   m_pSkillBtn = nullptr;
	CIndexIndicator*   m_pIndexIndicator = nullptr;

private:
	_bool m_arrCurWindow[INVEN_INDEX::TAB_END] = { false };

private:
	_float m_fInitPos = 10.f;
	_float m_fEndPos  = 0.f;
	_bool  m_bClickEquipment = false;
	_bool  m_bClickArmory = false;

public:
	static CUI_Indexes* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_Indexes_h__