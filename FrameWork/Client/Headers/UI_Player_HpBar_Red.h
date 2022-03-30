#pragma once

#ifndef UI_Player_HpBar_Red_h__
#define UI_Player_HpBar_Red_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
class CMouse;
END

BEGIN(Client)
class CUI_Player_HpBar_Red final : public CUI
{
private:
	explicit CUI_Player_HpBar_Red(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Player_HpBar_Red(const CUI& rhs);
	virtual ~CUI_Player_HpBar_Red() = default;

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

	_float m_fGapX = 0.f;
	_float m_fGapY = 0.f;
	_uint m_iCurScene = 0;

private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Player_HpBar_Red* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Player_HpBar_Red_h__
