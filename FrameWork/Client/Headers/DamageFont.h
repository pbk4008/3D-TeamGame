#pragma once

#include "GameObject.h"

BEGIN(Client)

class CDamageFont : public CGameObject
{
public:
	typedef struct tagDesc
	{
		_float3 vPos{};
		_float fDamage = 0.f;
	}DESC;
private:
	explicit CDamageFont(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CDamageFont(const CDamageFont& _rhs);
	virtual ~CDamageFont() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

private:
	CTexture* m_pTexture = nullptr;
	vector<pair<CVIBuffer_Rect*, CTexture*>> m_vecFonts;
	vector<_uint> m_vecIndex;
	vector<CTransform*> m_vecLocalTransforms;
	vector<CTransform*> m_vecWorldTransforms;
	_float3 m_vPos{};
	_float m_fDamage = 0.f;

	_uint m_iFontSize = 0;

public:
	static CDamageFont* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END