#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Component.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CCamera final : public CComponent
{
public:
	enum class CAMERATYPE {CAMERA_ORTHO, CAMERA_PROJECTION};
	typedef struct tagCameraDesc
	{
		CAMERATYPE eType;
		_float4 vEye, vAt, vAxisY;
		_float fAspect, fFovy, fNear, fFar;
		_float fWinCX, fWinCY;
		wstring pCameraTag;
		tagCameraDesc() = default;
		tagCameraDesc(const tagCameraDesc& rhs)
			: eType(rhs.eType)
			, vEye(rhs.vEye)
			, vAt(rhs.vAt)
			, vAxisY(rhs.vAxisY)
			, fAspect(rhs.fAspect)
			, fFovy(rhs.fFovy)
			, fNear(rhs.fNear)
			, fFar(rhs.fFar)
			, fWinCX(rhs.fWinCX)
			, fWinCY(rhs.fWinCY)
			, pCameraTag(rhs.pCameraTag)
		{
		}
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
	const wstring& getTag() { return m_pCameraDesc->pCameraTag; }
private:
	virtual void Free() override;
private:
	_float4x4 m_matView;
	_float4x4 m_matProj;
	CAMERADESC* m_pCameraDesc;
};
END
#endif