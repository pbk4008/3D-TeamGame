#pragma once
#ifndef __MESHCOLLIDER_H__
#define __MESHCOLLIDER_H__

#include "Collider.h"
BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CMeshCollider final : public CCollider
{
public:
	typedef struct tagMeshDesc
	{
		_float4x4 matTransform;
		CPhysicsXSystem::COLDESC tColDesc;
		vector<_float3*> vecPoints;
		CGameObject* pParent;
	}MESHDESC;
private:
	explicit CMeshCollider() = default;
	explicit CMeshCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMeshCollider(const CMeshCollider& rhs);
	virtual ~CMeshCollider() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual HRESULT Render(const wstring& pCameraTag);
private:
	virtual HRESULT Init_Shape(const vector<_float3*>& vecPoints);
public:
	static CMeshCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg);
private:
	virtual void Free() override;
};
END
#endif