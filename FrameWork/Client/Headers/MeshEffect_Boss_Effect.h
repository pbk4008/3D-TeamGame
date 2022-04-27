#pragma once

#include "MeshEffect.h"

BEGIN(Client)

class CMeshEffect_Boss_Effect : public CMeshEffect
{
private:
	explicit CMeshEffect_Boss_Effect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMeshEffect_Boss_Effect(const CMeshEffect_Boss_Effect& _rhs);
	virtual ~CMeshEffect_Boss_Effect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

private:
	CTexture* m_pMaskTexture;


public:
	static CMeshEffect_Boss_Effect* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr);
	virtual void Free() override;
};

END