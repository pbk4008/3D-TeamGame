#ifndef UI_EquippedWeapon_Slot_1_h__
#define UI_EquippedWeapon_Slot_1_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_EquippedWeapon_Slot_1 : public CUI
{
public:
	struct Desc
	{

	};

	explicit CUI_EquippedWeapon_Slot_1(void) = default;
	explicit CUI_EquippedWeapon_Slot_1(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_EquippedWeapon_Slot_1(const CUI_EquippedWeapon_Slot_1& rhs);
	virtual ~CUI_EquippedWeapon_Slot_1() = default;


public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	void	SetImage(std::wstring szTexName);
	void	SetMainSlot(_bool OnOff) { m_IsMainSlot = OnOff;  }
	void	UpdateSlot(_double TimeDelta);
	void	UpdateSlotImage(_double TimeDelta);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	_bool		  m_IsMainSlot = false;
	_bool		  m_IsRightMoveEnd = false;
	_float		  m_fInitPos = 440.f;
	_float		  m_fEndPos =  460.f;

	_float		  m_fInitPos2 = 440.f;
	_float		  m_fEndPos2 = 460.f;

public:
	static CUI_EquippedWeapon_Slot_1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;

};
END


#endif // UI_EquippedWeapon_Slot_1_h__
