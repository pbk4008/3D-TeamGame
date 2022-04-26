#pragma once

#include "GameObject.h"

BEGIN(Client)

class CCapsuleObstacle : public CGameObject
{
public:
	typedef struct Desc
	{
		_float3 vPosition = { 0.f, 0.f, 0.f };
		_float3 vRotation = { 0.f, 0.f, 0.f };
		_float fRadius = 0.5f;
		_float fHeight = 1.f;
	}DESC;

private:
	explicit CCapsuleObstacle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCapsuleObstacle(const CCapsuleObstacle& _rhs);

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

	HRESULT Ready_Components();

private:
	CCapsuleCollider* m_pCollider = nullptr;
	DESC m_tDesc;

public:
	static CCapsuleObstacle* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END