#ifndef UI_Death_h__
#define UI_Death_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_Blank_Space;
class CUI_Fill_Space;
class CUI_Death  : public CUI
{
public:
	explicit CUI_Death(void) = default;
	explicit CUI_Death(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_Death(const CUI_Death& rhs);
	virtual ~CUI_Death() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	void HideSpaceKey(void);

private:
	CSingleImage*	m_pSigleImageCom = nullptr;

private: /* for. Death UI */
	CUI_Blank_Space* m_pBlankSpaceKey = nullptr;
	CUI_Fill_Space* m_pFillSpaceKey = nullptr;

public:
	static CUI_Death* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_Death_h__


