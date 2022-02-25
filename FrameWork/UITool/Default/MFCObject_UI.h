#pragma once

#ifndef MFCObject_UI_h__
#define MFCObject_UI_h__

#include "UI.h"
#include "UI_Tool_Define.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
END

class CMFCObject_UI final : public CUI
{
private:
	explicit CMFCObject_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMFCObject_UI(const CUI& rhs);
	virtual ~CMFCObject_UI() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Show(_bool Show) { m_bShow = Show; }
	_bool Get_isIn() { return m_bIn; }
	_uint Get_CurrentImage() { return m_iCurrentImage; }
	void Set_CurrentImage(_uint iImage) { m_iCurrentImage = iImage; }

private:
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

private:
	_bool m_bIn = true; //마우스가 얘 안에 들어와있으면 충돌중이라는걸 return하기위해 
	_uint m_iCurrentImage = 0; //기본 0, 마우스가 만두랑 충돌하면 1로 변경

private:
	_float4x4	m_TransformMatrix;
	_matrix	m_ProjectionMatrix;
	_float4x4	m_WorldMatrix;
	_float m_fX, m_fY;
	_float m_fSizeX, m_fSizeY;

private:
	_bool m_bShow = false;

private:
	virtual HRESULT SetUp_Components();

public:
	static CMFCObject_UI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

#endif // MFCObject_UI_h__
