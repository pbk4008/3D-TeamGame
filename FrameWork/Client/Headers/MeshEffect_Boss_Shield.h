#pragma once

#include "MeshEffect.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)

class CMeshEffect_Boss_Shield : public CMeshEffect
{
private:
	explicit CMeshEffect_Boss_Shield(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMeshEffect_Boss_Shield(const CMeshEffect_Boss_Shield& _rhs);
	virtual ~CMeshEffect_Boss_Shield() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

public: void Set_DissolveON(_bool check) { m_bdissolveOn = check; }

public: void Set_DissolveOFF(_bool check) {
	m_bdissolveOff = check; 
	m_bActive = true;
	}

private:
	HRESULT Ready_Components();
	void	DissolveOn(_float dissolvespeed);
	void	DissolveOff(_float dissolvespeed);

private:
	_bool m_bBeSmall = false; //한번커지고난뒤에 작아져야할때 

	CTexture*	m_dissolveTex = nullptr;
	_float		m_lifetime = 0.f;
	_bool		m_bdissolveOn = false;
	_bool		m_bdissolveOff = false;
public:
	static CMeshEffect_Boss_Shield* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr);
	virtual void Free() override;
};

END