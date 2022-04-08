#pragma once
#include "LandScape.h"

BEGIN(Client)
class CSubEnvironment final : public CLandScape
{
public:
	typedef struct tagEnvironmentDesc
	{
		wstring wstrInstaneTag = L"";
		CInstancing_Mesh::INSTANCINGMESHDESC tInstanceDesc;
	}ENVIRONMENTDESC;
private:
	explicit CSubEnvironment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CSubEnvironment(const CSubEnvironment& rhs);
	virtual ~CSubEnvironment() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render()override;
	virtual HRESULT	Render_Shadow() override;
private:
	HRESULT Ready_Component();
	HRESULT Culling();
public:
	static CSubEnvironment* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CSubEnvironment* Clone(const _uint _iSceneID, void* pArg);
private:
	virtual void Free() override;
private:
	ENVIRONMENTDESC		m_tEnvironmentDesc;
	CInstancing_Mesh* m_pInstanceMesh;
	const LIGHTDESC* m_LightDesc = nullptr;
	_ulong				m_Nummeshcontainer = 0;
	_uint					m_iInstanceCnt;
	vector<_float4x4> m_vecUsingMatrix;
};
END
