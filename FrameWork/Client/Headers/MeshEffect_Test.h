#pragma once

#include "MeshEffect.h"

BEGIN(Client)

class CMeshEffect_Test : public CMeshEffect
{
private:
	explicit CMeshEffect_Test(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMeshEffect_Test(const CMeshEffect_Test& _rhs);
	virtual ~CMeshEffect_Test() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

public:
	static CMeshEffect_Test* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr);
	virtual void Free() override;
};

END