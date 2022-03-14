#pragma once

#include "Weapon.h"

BEGIN(Client)

class CFury final : public CWeapon
{
private:
	explicit CFury(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CFury(const CFury& _rhs);
	virtual ~CFury() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	_int Attach_FixedBone(const _double& _dDeltaTime);
	_int Attach_Owner(const _double& _dDeltaTime);

private:
	_matrix m_smatPivot = XMMatrixIdentity();

public:
	static CFury* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END