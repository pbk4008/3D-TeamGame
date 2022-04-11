#ifndef UI_SlotItemEffect_h__
#define UI_SlotItemEffect_h__

#include "UI.h"

BEGIN(Client)

class CUI_SlotItemEffect :public CUI
{
public:
	static CUI_SlotItemEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_SlotItemEffect_h__

