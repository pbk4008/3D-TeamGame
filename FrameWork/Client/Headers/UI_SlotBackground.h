#ifndef UI_SlotBackground_h__
#define UI_SlotBackground_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_SlotBackground : public CUI
{
public:
	struct ItemSlotDesc
	{
		_float2 fPos;
		_float2 fScale;
	};

	explicit CUI_SlotBackground(void) = default;
	explicit CUI_SlotBackground(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_SlotBackground(const CUI_SlotBackground& rhs);
	virtual ~CUI_SlotBackground() = default;

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
	ItemSlotDesc  desc;

public:
	static CUI_SlotBackground* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_SlotBackground_h__
