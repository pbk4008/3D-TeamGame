#pragma once

#include "GameObject.h"

BEGIN(Client)

class CMeshEffect abstract : public CGameObject
{
public:
	typedef struct tagMeshEffectDesc
	{
		_float3 vPosition{};
		_float fLifeTime = 0.f;
	}DESC;
protected:
	explicit CMeshEffect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMeshEffect(const CMeshEffect& _rhs);
	virtual ~CMeshEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

protected:
	// Componnents
	CModel* m_pModel = nullptr;
	CMaterial* m_pMaterial = nullptr;
	CTexture* m_pTexture = nullptr;

	// Time
	_float m_fLifeTime = 0.f;
	_float m_fAccTime = 0.f;

	// Ahlpha
	_float m_fAlpha = 0.f;
	_float m_fFlowSpeedAlpha = 0.f;

	// Position
	_float3 m_vStartPosition{};
	_float3 m_vEndPosition{};
	_float3 m_vPosition{};

	// Scale
	_float3 m_vStartScale{};
	_float3 m_vEndScale{};
	_float3 m_vScale{};

	// UV
	_bool m_isReverse = false;
	_bool m_isFlowX = false;
	_bool m_isFlowY = false;
	_float m_fFlowSpeedX = 0.f;
	_float m_fFlowSpeedY = 0.f;
	_float2 m_vPlusUV{};
	_float2 m_vTiling = { 1.f, 1.f };

	// Color
	_bool m_isCustomColor = false;
	_float3 m_vColor{};

public:
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override PURE;
	virtual void Free() override;
};

END