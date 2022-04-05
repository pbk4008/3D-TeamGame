#pragma once

#include "GameObject.h"

BEGIN(Client)

class CTrailEffect : public CGameObject
{
private:
	explicit CTrailEffect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CTrailEffect(const CTrailEffect& _rhs);
	virtual ~CTrailEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

public:
	void Set_IsRender(const _bool _isRender);
	void Record_Points(const _double& _dDeltaTIme);
	void Clear_Points();
	void CatmullRom();

private:
	CVIBuffer_Trail* m_pVIBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CTexture* m_pTexture1 = nullptr;
	CTexture* m_pTexture2 = nullptr;

	//list<_float3> m_listPoints;
	list<pair<_float3, _float3>> m_listPoints; // first : pos, second : dir
	list<pair<_float3, _float3>> m_listCurved;

	_float m_fAccTime = 0.f;
	_float m_fLength = 0.f;

	CTransform* m_pTargetTransform = nullptr;

	_bool m_isRender = false;
	_float4x4 m_matPivot{};

public:
	static CTrailEffect* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END