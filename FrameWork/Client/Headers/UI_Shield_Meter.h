#pragma once

#ifndef UI_Shield_Meter_h__
#define UI_Shield_Meter_h__

#include "UI_Monster_Panel.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Shield_Meter final : public CUI_Monster_Panel
{
public:
	typedef struct tagUIDesc
	{
		CUI::UIDESC UIDesc;
	}UIACTIVEDESC;

private:
	explicit CUI_Shield_Meter(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Shield_Meter(const CUI_Monster_Panel& rhs);
	virtual ~CUI_Shield_Meter() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_UIDesc(UIACTIVEDESC Desc) { m_Desc = Desc; }
	UIACTIVEDESC Get_UIDesc() { return m_Desc; }

private:
	UIACTIVEDESC m_Desc;
	_bool m_bUseShield = false; //false일때는 다 차있다가 쓰면 제로부터시작..
	_uint m_iTextureNum = 0;
	_float m_fFillTimeAcc = 0.f;

	_bool m_bCoolTime = false;
	_float m_fCoolTimeAcc = 0.f;

private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Shield_Meter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Shield_Meter_h__
