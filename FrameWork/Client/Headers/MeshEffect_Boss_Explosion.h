#pragma once

#include "MeshEffect.h"

BEGIN(Client)

class CMeshEffect_Boss_Explosion : public CMeshEffect
{
private:
	explicit CMeshEffect_Boss_Explosion(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMeshEffect_Boss_Explosion(const CMeshEffect_Boss_Explosion& _rhs);
	virtual ~CMeshEffect_Boss_Explosion() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

private:
	_bool m_bBeSmall = false; //한번커지고난뒤에 작아져야할때 

public:
	static CMeshEffect_Boss_Explosion* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr);
	virtual void Free() override;
};

END