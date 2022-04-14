#ifndef Hud_h__
#define Hud_h__

#include "UI.h"

BEGIN(Client)
class CLoot_Equipment;
class CHud : public CUI
{
public:
	explicit CHud(void) = default;
	explicit CHud(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CHud(const CHud& rhs);
	virtual ~CHud() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual void setActive(_bool bActive) override;

public:
	virtual HRESULT Ready_Component(void);
	virtual HRESULT Ready_UIObject(void);

public:
	void PullingEquipmentUI(void);

public:
	void OnLootEquipment(void* pItemData);

private:
	CLoot_Equipment* m_pLootEquipment = nullptr;

private:
	std::vector< CLoot_Equipment*> m_vecLootEquipment;

public:
	static CHud* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr);

private:
	virtual void Free() override;
};

END
#endif // Hud_h__
