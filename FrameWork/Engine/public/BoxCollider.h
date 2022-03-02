#pragma once
#ifndef __BOXCOLLIDER_H__
#define __BOXCOLLIDER_H__

#include "Collider.h"

BEGIN(Engine)
class CGameObject;
class CGizmo;
class ENGINE_DLL CBoxCollider final : public CCollider
{
public:
	typedef struct tagBox
	{
		_float4x4 matTransform;
		CPhysicsXSystem::COLDESC tColDesc;
		CGameObject* pParent;
	}BOXDESC;
private:
	explicit CBoxCollider() = default;
	explicit CBoxCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoxCollider(const CBoxCollider& rhs);
	virtual ~CBoxCollider() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual HRESULT Render(const wstring& pCameraTag);
private:
	virtual HRESULT Init_Shape(PxVec3 pxExtends);
public:
	static CBoxCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg);
private:
	virtual void Free() override;
};
END
#endif