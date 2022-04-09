#ifndef Staff_h__
#define Staff_h__

#include "Weapon.h"

BEGIN(Client)

class CStaff final : public CWeapon
{
private:
	explicit CStaff(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CStaff(const CStaff& _rhs);
	virtual ~CStaff() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;

private:
	HRESULT Ready_Components();
	_int Attach_FixedBone(const _double& _dDeltaTime);
	_int Attach_Owner(const _double& _dDeltaTime);

private:
	_matrix m_smatPivot = XMMatrixIdentity();

public:
	static CStaff* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END

#endif // Staff_h__