#pragma once

#include "TrailEffect.h"

BEGIN(Client)

class CTrailEffect_Distortion : public CTrailEffect
{
private:
	explicit CTrailEffect_Distortion(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CTrailEffect_Distortion(const CTrailEffect_Distortion& _rhs);
	virtual ~CTrailEffect_Distortion() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

private:
	CTexture* m_pTexture1 = nullptr;
	CTexture* m_pTexture2 = nullptr;

private:
	_float m_Frametime = 0.f;

public:
	static CTrailEffect_Distortion* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END