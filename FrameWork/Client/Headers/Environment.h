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
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render()override;
private:
	HRESULT Ready_Component();
public:
	static CEnvironment* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
private:
	virtual void Free() override;
private:
	ENVIRONMENTDESC m_tEnvironmentDesc;
	CMeshCollider* m_pNaviMesh;
	CInstancing_Mesh* m_pInstanceMesh;
};
END
#endif