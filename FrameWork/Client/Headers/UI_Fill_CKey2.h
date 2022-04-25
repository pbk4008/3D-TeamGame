#pragma once

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Fill_Ckey2 final : public CUI
{
public:
	typedef struct tagUIDesc
	{
		CUI::UIDESC UIDesc;
		_tchar TextureTag2[MAX_PATH];
	}UIACTIVEDESC;

private:
	explicit CUI_Fill_Ckey2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Fill_Ckey2(const CUI_Fill_Ckey2& rhs);
	virtual ~CUI_Fill_Ckey2() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_UIDesc(UIACTIVEDESC Desc) { m_Desc = Desc; }
	UIACTIVEDESC Get_UIDesc() { return m_Desc; }
	void Set_GapX(_float fGap) { m_fGapX = fGap; }
	void Set_Press(_bool _isPress) { m_isPress = _isPress; }
	void Set_Position(const _fvector& _svPos);

private:
	UIACTIVEDESC m_Desc;
	_float m_fGapX = 0.f;
	_float m_fGapY = 1.f;
	_float m_fAlpha = 1.f;
	_bool m_isPress = false;

private:
	CVIBuffer_Rect* m_pBuffer2 = nullptr;
	CTexture* m_pTexture2 = nullptr;

private:
	virtual HRESULT Ready_Components();

public:
	static CUI_Fill_Ckey2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
