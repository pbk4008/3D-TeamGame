#pragma once

#include "GameObject.h"

BEGIN(Client)

class CBoxBridge : public CGameObject
{
public:
	typedef struct Desc
	{
		_float3 vPosition = { 0.f, 0.f, 0.f };
		_float3 vRotation = { 0.f, 0.f, 0.f };
		_float3 vScale = { 0.f, 0.f, 0.f };
	}DESC;

private:
	explicit CBoxBridge(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBoxBridge(const CBoxBridge& _rhs);

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

	HRESULT Ready_Components();

private:
	CBoxCollider* m_pCollider = nullptr;
	DESC m_tDesc;

public:
	static CBoxBridge* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END