#pragma once

#ifndef UI_Skill_Meter_Gauge_h__
#define UI_Skill_Meter_Gauge_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
class CMouse;
END

BEGIN(Client)
class CUI_Skill_Meter_Gauge final : public CUI
{
private:
	explicit CUI_Skill_Meter_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Skill_Meter_Gauge(const CUI& rhs);
	virtual ~CUI_Skill_Meter_Gauge() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	_float GetRatio() { return m_fRatio; }
	void Set_UIDesc(UIDESC Desc) { m_Desc = Desc; }
	UIDESC Get_UIDesc() { return m_Desc; }
	const _bool GetStateFull() const { return m_bFull; } //게이지가 다 차있는지 아닌지 
private:
	UIDESC m_Desc;
	
	_float m_fRatio = 0.f;

	_float m_fGapX = 0.f;
	_float m_fGapY = 0.f;

	_bool m_bFull = true; //게이지가 다 차있으면 true 

private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Skill_Meter_Gauge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Skill_Meter_Gauge_h__
