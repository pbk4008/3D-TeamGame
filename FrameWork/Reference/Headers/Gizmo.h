#pragma once
#ifndef __GIZMO_H__
#define __GIZMO_H__
#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)
class ENGINE_DLL CGizmo final : public CComponent
{
private:
	explicit CGizmo();
	explicit CGizmo(ID3D11Device* pDevcie, ID3D11DeviceContext* pDeviceContext);
	explicit CGizmo(const CGizmo& rhs);
	virtual ~CGizmo() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	HRESULT DrawCube(_matrix matTransform, const wstring& pCameraTag, _fvector vColor);
	HRESULT DrawLine(_fvector vStart, _fvector vEnd, const wstring& pCameraTag, _fvector vColor);
	HRESULT DrawSphere(_matrix matTransform, const wstring& pCameraTag, _fvector vColor);
	HRESULT DrawCapsule(_matrix matTransform, const wstring& pCameraTag, _fvector vColor);
public:
	static CGizmo* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
private:
	virtual void Free() override;
private:
	BasicEffect* m_pEffect;
	PrimitiveBatch<VertexPositionColor>* m_pBatch;
	ID3D11InputLayout* m_pInputLayOut;
};
END
#endif