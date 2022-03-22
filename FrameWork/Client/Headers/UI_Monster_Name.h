#pragma once

#ifndef UI_Monster_Name_h__
#define UI_Monster_Name_h__

#include "UI_Monster_Panel.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Monster_Name final : public CUI_Monster_Panel
{
public:
	typedef struct tagUIDesc
	{
		CUI::UIDESC UIDesc;
		_uint iTextureNum;
	}UINAMEDESC;

private:
	explicit CUI_Monster_Name(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Monster_Name(const CUI_Monster_Panel& rhs);
	virtual ~CUI_Monster_Name() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_UIDesc(UINAMEDESC Desc) { m_Desc = Desc; }
	UINAMEDESC Get_UIDesc() { return m_Desc; }

private:
	UINAMEDESC m_Desc;
private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Monster_Name* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Monster_Name_h__
