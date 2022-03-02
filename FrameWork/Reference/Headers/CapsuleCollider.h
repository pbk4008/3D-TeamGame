#pragma once
#ifndef __CAPSULECOLLIDER_H__
#define __CAPSULECOLLIDER_H__
#include "Collider.h"
BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CCapsuleCollider final : public CCollider
{
public:
	typedef struct tagCapsule
	{
		_float4x4 matTransform;
		CPhysicsXSystem::COLDESC tColDesc;
		CGameObject* pParent;
	}CAPSULEDESC;
private:
	explicit CCapsuleCollider() = default;
	explicit CCapsuleCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCapsuleCollider(const CCapsuleCollider& rhs);
	virtual ~CCapsuleCollider() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual HRESULT Render(const wstring& pCameraTag);
private:
	virtual HRESULT Init_Shape(PxVec3 pxExtends);
public:
	static CCapsuleCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg);
private:
	virtual void Free() override;
};
END
#endif