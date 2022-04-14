#pragma once

#include "GameObject.h"

BEGIN(Client)

class CTrailEffect abstract : public CGameObject
{
public:
	typedef struct tagTrailDesc
	{
		CTransform* pOwnerTransform = nullptr;
		_float fLength = 0.f;
		_float4x4 matPivot{};
		wstring wstrTextureTag = L"TrailBase";
	}DESC;
protected:
	explicit CTrailEffect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CTrailEffect(const CTrailEffect& _rhs);
	virtual ~CTrailEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

protected:
	void CatmullRom();

public:
	void Set_IsRender(const _bool _isRender);
	void Set_Length(const _float _fLength);
	void Set_Texture(const wstring& _wstrTextureTag);
	void Set_PivotMatrix(const _fmatrix& _smatPivot);
	void Record_Points(const _double& _dDeltaTIme);
	void Clear_Points();

protected:
	CVIBuffer_Trail* m_pVIBuffer = nullptr;
	CTexture* m_pTexture = nullptr;

	_bool m_isRender = false;
	_float m_fAccTime = 0.f;

	list<pair<_float3, _float3>> m_listPoints; // first : start, second : end
	list<pair<_float3, _float3>> m_listCurved;

	DESC m_tDesc;

public:
	virtual CGameObject* Clone(_uint _iSceneID, void* _pArg = nullptr) PURE;
	virtual void Free() override;
};

END