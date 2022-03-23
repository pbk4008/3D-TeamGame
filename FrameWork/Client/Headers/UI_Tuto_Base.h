#pragma once

#ifndef UI_Tuto_Base_h__
#define UI_Tuto_Base_h__

#include "UI_Monster_Panel.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Tuto_Base final : public CUI_Monster_Panel
{
public:
	typedef struct tagUIDesc
	{
		CUI::UIDESC UIDesc;
	}UIACTIVEDESC;

private:
	explicit CUI_Tuto_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Tuto_Base(const CUI_Monster_Panel& rhs);
	virtual ~CUI_Tuto_Base() = default;

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
private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Tuto_Base* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Tuto_Base_h__
