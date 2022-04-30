#pragma once

#ifndef UI_Monster_LevelNum_h__
#define UI_Monster_LevelNum_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Monster_LevelNum final : public CUI
{
public:
	typedef struct tagUIDesc
	{
		CUI::UIDESC UIDesc;
		_uint iTextureNum;
	}UIACTIVEDESC;

private:
	explicit CUI_Monster_LevelNum(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Monster_LevelNum(const CUI_Monster_LevelNum& rhs);
	virtual ~CUI_Monster_LevelNum() = default;

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
	_float m_fAlpha = 1.f;
	_float m_fDisappearTimeAcc = 0.f;

private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Monster_LevelNum* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Monster_LevelNum_h__
