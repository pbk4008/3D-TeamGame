#pragma once

#include "GameObject.h"

BEGIN(Client)

class CSilvermane;
class CCameraShake;

class CCamera_Silvermane final : public CGameObject
{
	friend class CCamera_Culling;
	friend class CCameraShake;
private:
	explicit CCamera_Silvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCamera_Silvermane(const CCamera_Silvermane& _rhs);
	virtual ~CCamera_Silvermane() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

public:
	void Set_ChaseTarget(const _bool _isChase);

private:
	_int Chase_Target(const _double& _dDeltaTime);
	_int Input_Key(const _double& _dDeltaTime);
	void SpringArm();

public:
	const _fvector Get_Look() const;

private:
	CCamera* m_pCamera = nullptr;
	CSilvermane* m_pSilvermane = nullptr;
	CTransform* m_pLocalTransform = nullptr;
	CTransform* m_pWorldTransform = nullptr;

	_float3 m_vRot = { 0.f,0.f,0.f };
	_float3 m_vLocalOriginPos = { 0.f, 0.f, 0.f };
	_float3 m_vShakeAccPos = { 0.f, 0.f, 0.f };

	_bool m_isChase = true;
	_float m_fLerpTime = 0.f;

private: /* For.Shake */
	CCameraShake* m_pCameraShake = nullptr;

public:
	static CCamera_Silvermane* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg) override;
	virtual void Free() override;
};

END