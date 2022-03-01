#pragma once
#ifndef __BOXCOLLIDER_H__
#define __BOXCOLLIDER_H__

#include "Collider.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CBoxCollider final : public CCollider
{
public:
	enum class ROTATE {ROT_AABB, ROT_OBB, ROT_END};
public:
	typedef struct tagBox
	{
		ROTATE eRot;
		_float3 fExtends;
		CPhysicsXSystem::COLDESC tColDesc;
		CGameObject* pParent;
	}BOXDESC;
private:
	explicit CBoxCollider();
	explicit CBoxCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoxCollider(const CBoxCollider& rhs);
	virtual ~CBoxCollider() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	void Update(_fmatrix TransformMatrix);
	virtual HRESULT Render(const wstring& pCameraTag);
private:
	virtual HRESULT Init_Shape(PxVec3 pxExtends);
	HRESULT Init_Box();
public:
	static CBoxCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg);
private:
	virtual void Free() override;
private:
	BoundingBox* m_pOriginAABB;
	BoundingBox* m_pAABB;

	BoundingOrientedBox* m_pOriginOBB;
	BoundingOrientedBox* m_pOBB;
private:
	ROTATE m_eRotateType;
};
END
#endif