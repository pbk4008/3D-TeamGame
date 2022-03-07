#pragma once

#include "GameObject.h"

BEGIN(Client)

class CSilvermane;

class CCamera_Silvermane final : public CGameObject
{
private:
	explicit CCamera_Silvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCamera_Silvermane(const CCamera_Silvermane& _rhs);
	virtual ~CCamera_Silvermane() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
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

private:
	CCamera* m_pCamera = nullptr;
	CSilvermane* m_pSilvermane = nullptr;
	CTransform* m_pLocalTransform = nullptr;
	CTransform* m_pWorldTransform = nullptr;

	_float m_fRotY = 0.f;

	_bool m_isChase = true;

public:
	static CCamera_Silvermane* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END