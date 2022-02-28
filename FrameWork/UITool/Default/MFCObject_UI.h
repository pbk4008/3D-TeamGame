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
class CMouse;
END

class CMFCObject_UI final : public CUI
{
public:
	typedef struct tagUIDesc
	{
		 _tchar TextureTag[MAX_PATH];
		_float2 fPos;
		_float2 fScale;
	}UIDESC;

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
	void Set_UIDesc(UIDESC Desc) { m_Desc = Desc; }
	UIDESC Get_UIDesc() { return m_Desc; }

private:
	UIDESC m_Desc;

private:
	virtual HRESULT SetUp_Components();

public:
	static CMFCObject_UI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

#endif // MFCObject_UI_h__
