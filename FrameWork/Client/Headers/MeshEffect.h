#pragma once

#include "GameObject.h"

BEGIN(Client)

class CMeshEffect : public CGameObject
{
public:
	typedef struct tagMeshEffectDesc
	{
		_float3 vPosition{};
		_float fLifeTime = 0.f;
	}DESC;
private:
	explicit CMeshEffect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMeshEffect(const CMeshEffect& _rhs);
	virtual ~CMeshEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

private:
	CModel* m_pModel = nullptr;

	DESC m_tDesc;

	_float m_fAccTime = 0.f;

	_float m_fAccScale = 0.f;
	_float m_fStartScale = 0.f;
	_float m_fEndScale = 0.f;

public:
	static CMeshEffect* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END