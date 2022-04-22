#pragma once

#include "Weapon.h"

BEGIN(Engine)
class CSplineCurve;
END

BEGIN(Client)

class CFlyingShield : public CWeapon
{
public:
	typedef struct tagDesc
	{
		_float3 vTargetPos{};
		CTransform* pOriginTransform = nullptr;
	}DESC;
private:
	explicit CFlyingShield(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CFlyingShield(const CFlyingShield& _rhs);
	virtual ~CFlyingShield() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;

	virtual void setActive(_bool bActive) override;

	virtual void OnTriggerEnter(CCollision& collision) override;
	void Throw(const _fvector& _svTargetPos);

private:
	HRESULT Ready_Components();

	void Return();

	void Spline_Throw();
	void Spline_Return();

private:
	CBoxCollider* m_pCollider = nullptr;
	CSplineCurve* m_pSpline = nullptr;

	DESC m_tDesc{};
	_bool m_isReturn = false;

	_float m_fAccTime = 0.f;
	_float m_fDis = 0.f;
	_float m_fAccDis = 0.f;
	_float m_fSpeed = 0.f;
	_float m_fLiveTime = 0.f;
	_float3 m_vDir{};

public:
	static CFlyingShield* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END