#ifndef UI_EquippedWeapon_h__
#define UI_EquippedWeapon_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_EquippedWeapon : public CUI
{
public:
	struct Desc
	{

	};

	explicit CUI_EquippedWeapon(void) = default;
	explicit CUI_EquippedWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_EquippedWeapon(const CUI_EquippedWeapon& rhs);
	virtual ~CUI_EquippedWeapon() = default;


public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	void	SetImage(std::wstring szTexName);

private:
	CSingleImage* m_pSigleImageCom = nullptr;

public:
	static CUI_EquippedWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;

};
END
#endif // UI_EquippedWeapon_h__