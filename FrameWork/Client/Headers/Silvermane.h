#pragma once

#include "Actor.h"

BEGIN(Engine)
END

BEGIN(Client)
class CWeapon;
class CCamera_Silvermane;

class CSilvermane final : public CActor
{
private:
	explicit CSilvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSilvermane(const CSilvermane& _rhs);
	virtual ~CSilvermane() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_States();

public:
	CTransform* Get_Transform() const;
	CModel* Get_Model() const;
	const _float Get_PlusAngle() const;

	void Set_EquipWeapon(const _bool _isEquipWeapon);
	void Set_WeaponFixedBone(CHierarchyNode* _pFixedBone);
	void Set_Camera(CCamera_Silvermane* _pCamera);

	const _bool Is_EquipWeapon() const;
	void Add_PlusAngle(const _float _fDeltaAngle);

private:
	_int Trace_CameraLook(const _double& _dDeltaTime);

private:
	CModel* m_pModel = nullptr;
	CStateController* m_pStateController = nullptr;
	CAnimationController* m_pAnimationController = nullptr;
	CCamera_Silvermane* m_pCamera = nullptr;

	CWeapon* m_pWeapon = nullptr;
	_bool m_isEquipWeapon = false;

	_float m_fAngle = 0.f;
	_float m_fPlusAngle = 0.f;
	_float m_fDeltaRadian = 0.f;

public:
	static CSilvermane* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END