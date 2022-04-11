#ifndef UI_Armory_h__
#define UI_Armory_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_Armory : public CUI
{
public:
	explicit CUI_Armory(void) = default;
	explicit CUI_Armory(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_Armory(const CUI_Armory& rhs);
	virtual ~CUI_Armory() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);

private:
	CSingleImage* m_pSigleImageCom = nullptr;

public:
	static CUI_Armory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_Armory_h__
