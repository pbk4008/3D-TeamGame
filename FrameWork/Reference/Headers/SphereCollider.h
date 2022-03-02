#pragma once
#ifndef __SPHERECOLLIDER_H__
#define __SPHERECOLLIDER_H__
#include "Collider.h"
BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CSphereCollider final : public CCollider
{
public:
	typedef struct tagSphere
	{
		_float4x4 matTransform;
		CPhysicsXSystem::COLDESC tColDesc;
		CGameObject* pParent;
	}SPHEREDESC;
private:
	explicit CSphereCollider();
	explicit CSphereCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CSphereCollider(const CSphereCollider& rhs);
	virtual ~CSphereCollider() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual HRESULT Render(const wstring& pCameraTag);
private:
	virtual HRESULT Init_Shape(PxVec3 pxExtends);
public:
	static CSphereCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg);
private:
	virtual void Free() override;
private:
	BoundingSphere* m_pOriginSphere;
	BoundingSphere* m_pSphere;
};
END
#endif