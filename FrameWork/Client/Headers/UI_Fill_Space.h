#ifndef UI_Fill_Space_h__
#define UI_Fill_Space_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Fill_Space final : public CUI
{
public:
	typedef struct tagUIDesc
	{
		CUI::UIDESC UIDesc;
	}UIACTIVEDESC;

private:
	explicit CUI_Fill_Space(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Fill_Space(const CUI_Fill_Space& rhs);
	virtual ~CUI_Fill_Space() = default;

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

private:
	UIACTIVEDESC m_Desc;
	_float m_fGapX = 0.f;
	_float m_fGapY = 1.f;
	_float m_fAlpha = 1.f;
	_float m_fPressTiime = 0.f;

private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Fill_Space* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END

#endif // UI_Fill_Space_h__
