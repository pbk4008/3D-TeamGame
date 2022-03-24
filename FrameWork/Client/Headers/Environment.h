#pragma once
#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__
#include "LandScape.h"
BEGIN(Engine)
class CMeshCollider;
END

BEGIN(Client)
class CEnvironment final : public CLandScape
{
public:
	typedef struct tagEnvironmentDesc
	{
		wstring wstrInstaneTag = L"";
		CInstancing_Mesh::INSTANCINGMESHDESC tInstanceDesc;
	}ENVIRONMENTDESC;
private:
	explicit CEnvironment();
	explicit CEnvironment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEnvironment(const CEnvironment& rhs);
	virtual ~CEnvironment() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render()override;
	virtual HRESULT	Render_Shadow() override;
	virtual HRESULT	Render_ShadeShadow(ID3D11ShaderResourceView* ShadowMap) override;
private:
	HRESULT Ready_Component();
	HRESULT Culling();
public:
	static CEnvironment* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg);
private:
	virtual void Free() override;
private:
	ENVIRONMENTDESC		m_tEnvironmentDesc;
	CInstancing_Mesh*	m_pInstanceMesh;
	const LIGHTDESC*	m_LightDesc = nullptr;
	_ulong				m_Nummeshcontainer = 0;
	_uint					m_iInstanceCnt;
	vector<_float4x4> m_vecUsingMatrix;
};
END
#endif