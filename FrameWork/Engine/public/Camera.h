#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Component.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CCamera final : public CComponent
{
public:
	enum class CAMERATYPE {CAMERA_ORTHO, CAMERA_PROJECTION, CAMERA_END};
	typedef struct tagCameraDesc
	{
		CAMERATYPE eType = CAMERATYPE::CAMERA_END;
		_float4 vEye = _float4(0.f,0.f,0.f,0.f);
		_float4 vAt = _float4(0.f,0.f,0.f,0.f);
		_float4 vAxisY = _float4(0.f,0.f,0.f,0.f);
		_float fAspect = 0.f;
		_float fFovy = 0.f;
		_float fNear = 0.f;
		_float fFar = 0.f;
		_float fWinCX = 0.f;
		_float fWinCY = 0.f;
		wstring pCameraTag=L"";
		tagCameraDesc() = default;
	}CAMERADESC;
private:
	explicit CCamera();
	explicit CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
public:
	void Update_Matrix(_fmatrix& matWorld);
private:
	HRESULT setViewMatrix();
	HRESULT setProjMatrix();
public:
	virtual CCamera* Clone(void* pArg);
	static CCamera* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context);
public:
	_uint getType();
	_fmatrix getProjection() { return XMLoadFloat4x4(&m_matProj); }
	const wstring& getTag() { return m_tCameraDesc.pCameraTag; }
	const CAMERADESC GetDesc() const { return m_tCameraDesc; }
	void setDesc(const CAMERADESC& _tDesc);

private:
	virtual void Free() override;
private:
	_float4x4 m_matView;
	_float4x4 m_matProj;
	CAMERADESC m_tCameraDesc;
};
END
#endif