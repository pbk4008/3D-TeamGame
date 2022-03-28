#pragma once

#include "GameObject.h"

BEGIN(Client)

class CJumpBox : public CGameObject
{
public:
	typedef struct Desc
	{
		_float3 vPosition = { 0.f, 0.f, 0.f };
		_float3 vRotation = { 0.f, 0.f, 0.f };
		_float3 vScale = { 0.f, 0.f, 0.f };
	}DESC;

private:
	explicit CJumpBox(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CJumpBox(const CJumpBox& _rhs);

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

	HRESULT Ready_Components();

	void DisableCollision();

private:
	CBoxCollider* m_pCollider = nullptr;
	_bool m_isPick = false;
	_bool m_isDisable = false;
	_float m_fDisableTime = 0.f;

	DESC m_tDesc;

public:
	static CJumpBox* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END