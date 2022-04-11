#ifndef UI_SlotGrade_h__
#define UI_SlotGrade_h__

#include "UI.h"

BEGIN(Client)

class CUI_SlotGrade final : public CUI
{
public:
	static CUI_SlotGrade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};
END
#endif // UI_SlotGrade_h__
