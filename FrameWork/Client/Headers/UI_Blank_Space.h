#ifndef UI_Blank_Space_h__
#define UI_Blank_Space_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Blank_Space final : public CUI
{
public:
	typedef struct tagUIDesc
	{
		CUI::UIDESC UIDesc;
	}UIACTIVEDESC;

private:
	explicit CUI_Blank_Space(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Blank_Space(const CUI_Blank_Space& rhs);
	virtual ~CUI_Blank_Space() = default;

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
	static CUI_Blank_Space* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END


#endif // UI_Blank_Space_h__