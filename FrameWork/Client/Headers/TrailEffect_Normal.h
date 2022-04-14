#pragma once

#include "TrailEffect.h"

BEGIN(Client)

class CTrailEffect_Normal : public CTrailEffect
{
private:
	explicit CTrailEffect_Normal(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CTrailEffect_Normal(const CTrailEffect_Normal& _rhs);
	virtual ~CTrailEffect_Normal() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

public:
	static CTrailEffect_Normal* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext);
	virtual CGameObject* Clone(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END