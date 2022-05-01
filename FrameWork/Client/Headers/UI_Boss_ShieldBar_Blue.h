#pragma once

#ifndef UI_Boss_ShieldBar_Blue_h__
#define UI_Boss_ShieldBar_Blue_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
class CMouse;
END

BEGIN(Client)
class CUI_Boss_ShieldBar_Blue final : public CUI
{
private:
	explicit CUI_Boss_ShieldBar_Blue(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Boss_ShieldBar_Blue(const CUI& rhs);
	virtual ~CUI_Boss_ShieldBar_Blue() = default;

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

private:
	UIDESC m_Desc;
	
	_float m_fRatio = 0.f;

	_float m_fGapX = 0.f;
	_float m_fGapY = 1.f;

	_float m_fBossShield = 0.f;
	_float m_fBossMaxShield = 0.f;
private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Boss_ShieldBar_Blue* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Boss_ShieldBar_Blue_h__
