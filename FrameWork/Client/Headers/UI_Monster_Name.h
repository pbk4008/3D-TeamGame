#pragma once

#ifndef UI_Monster_Name_h__
#define UI_Monster_Name_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Monster_Name final : public CUI
{
public:
	typedef struct tagUIDesc
	{
		CUI::UIDESC UIDesc;
		_uint iTextureNum;
	}UINAMEDESC;

private:
	explicit CUI_Monster_Name(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Monster_Name(const CUI_Monster_Name& rhs);
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
	_float m_fAlpha = 1.f;
	_float m_fDisappearTimeAcc = 0.f;

	_bool m_bAutoDis = false; //자동으로꺼질수있게
	_float m_fAutoDisTimeAcc = 0.f; //자동으로꺼질시간

private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Monster_Name* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Monster_Name_h__
