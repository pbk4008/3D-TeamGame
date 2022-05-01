#pragma once

#ifndef UI_Bar_Mark_h__
#define UI_Bar_Mark_h__

#include "UI_Monster_Panel.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Bar_Mark final : public CUI_Monster_Panel
{
public:
	typedef struct tagUIDesc : public CUI::UIDESC
	{
		_uint iTextureNum;
	}UIACTIVEDESC;

private:
	explicit CUI_Bar_Mark(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Bar_Mark(const CUI_Monster_Panel& rhs);
	virtual ~CUI_Bar_Mark() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_UIDesc(UIACTIVEDESC Desc) { m_Desc = Desc; }
	UIACTIVEDESC Get_UIDesc() { return m_Desc; }

public:
	void Set_HpHealNum(_uint iNum) { m_Desc.iTextureNum = iNum; }

private:
	UIACTIVEDESC m_Desc;

	class CInventoryData* m_pInventoryData = nullptr;

private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Bar_Mark* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Bar_Mark_h__
