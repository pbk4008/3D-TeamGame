#pragma once

#ifndef UI_Monster_Back_h__
#define UI_Monster_Back_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Trapezium;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Monster_Back final : public CUI
{
public:
	typedef struct tagUIDesc
	{
		CUI::UIDESC UIDesc;
	}UIACTIVEDESC;

private:
	explicit CUI_Monster_Back(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Monster_Back(const CUI_Monster_Back& rhs);
	virtual ~CUI_Monster_Back() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_UIDesc(UIACTIVEDESC Desc) { m_Desc = Desc; }
	UIACTIVEDESC Get_UIDesc() { return m_Desc; }
	void Set_GapY(_float GapY) { m_fGapY = GapY; }
private:
	UIACTIVEDESC m_Desc;
	_float m_fGapX = 0.f;
	_float m_fGapY = 0.f;
private:
	virtual HRESULT SetUp_Components();
public:
	static CUI_Monster_Back* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Monster_Back_h__
