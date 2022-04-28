#ifndef RetributionBlade_h__
#define RetributionBlade_h__

#include "Weapon.h"

BEGIN(Client)

class CTrailEffect_Distortion;

class CRetributionBlade final : public CWeapon
{
private:
	explicit CRetributionBlade(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CRetributionBlade(const CRetributionBlade& _rhs);
	virtual ~CRetributionBlade() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;

	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual void setActive(_bool bActive) override;

private:
	HRESULT Ready_Components();
	_int Attach_FixedBone(const _double& _dDeltaTime);
	_int Attach_Owner(const _double& _dDeltaTime);

private:
	_matrix m_smatPivot = XMMatrixIdentity();
	CCapsuleCollider* m_pCollider = nullptr;
	CTrailEffect_Distortion* m_pTrailEffect_Distortion = nullptr;

public:
	static CRetributionBlade* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END


#endif // RetributionBlade_h__