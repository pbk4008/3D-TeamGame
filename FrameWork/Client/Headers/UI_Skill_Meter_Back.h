#pragma once

#ifndef UI_Skill_Meter_Back_h__
#define UI_Skill_Meter_Back_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
class CMouse;
END

BEGIN(Client)
class CUI_Skill_Meter_Back final : public CUI
{
private:
	explicit CUI_Skill_Meter_Back(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Skill_Meter_Back(const CUI& rhs);
	virtual ~CUI_Skill_Meter_Back() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_UIDesc(UIDESC Desc) { m_Desc = Desc; }
	UIDESC Get_UIDesc() { return m_Desc; }

private:
	UIDESC m_Desc;
	_float m_fGapX = 1.f;
	_float m_fGapY = 1.f;
private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Skill_Meter_Back* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Skill_Meter_Back_h__
