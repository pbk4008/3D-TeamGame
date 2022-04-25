#ifndef UI_Guide_Background_h__
#define UI_Guide_Background_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_Guide_Background  : public CUI
{
public:
	explicit CUI_Guide_Background(void) = default;
	explicit CUI_Guide_Background(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_Guide_Background(const CUI_Guide_Background& rhs);
	virtual ~CUI_Guide_Background() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);

private:
	CSingleImage*	m_pSigleImageCom = nullptr;

public:
	static CUI_Guide_Background* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_Guide_Background_h__