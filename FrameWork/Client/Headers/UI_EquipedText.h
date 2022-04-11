#ifndef UI_EquipedText_h__
#define UI_EquipedText_h__

#include "UI.h"
BEGIN(Client)

class CUI_EquipedText : public CUI
{
public:
	static CUI_EquipedText* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_EquipedText_h__

