#pragma once

#ifndef UI_Indicator_h__
#define UI_Indicator_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Indicator final : public CUI
{
public:
	typedef struct tagUIDesc
	{
		CUI::UIDESC UIDesc;
	}UIACTIVEDESC;

private:
	explicit CUI_Indicator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Indicator(const CUI_Indicator& rhs);
	virtual ~CUI_Indicator() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_UIDesc(UIACTIVEDESC Desc) { m_Desc = Desc; }
	UIACTIVEDESC Get_UIDesc() { return m_Desc; }
	void Set_TargetPos(_vector TargetPos) { XMStoreFloat4(&m_TargetPos, TargetPos); }

private:
	UIACTIVEDESC m_Desc;
	_float m_fAlpha = 1.f;
	_float m_fDisappearTimeAcc = 0.f;

	_float4 m_TargetPos = { 0.f,0.f ,0.f , 1.f };

private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Indicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Indicator_h__
