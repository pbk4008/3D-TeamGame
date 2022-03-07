#pragma once
#ifndef __TOOLUI_H__
#define __TOOLUI_H__
#include "GameObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Tool)
class CToolUI final : public CGameObject
{
public:
	enum class UITAG { UI_BUTTON, UI_HPBAR, UI_PRINT, UI_LIFE,UI_END };
private:
	explicit CToolUI();
	explicit CToolUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CToolUI(const CToolUI& rhs);
	virtual ~CToolUI()=default;
public:
	virtual HRESULT Init_GameObject(COMINDEX* pIndex);
	virtual HRESULT Init_Clone(void* pArg);
	virtual _int Update_GameObject(_float fDeltaTime);
	virtual HRESULT Render_GameObject();
public:
	_bool Click_UI(_fvector vMousePos);
	void Change_Tranform(_fvector vScale, _fvector vPosition);
	void Change_ToolValue(_float2* vScale, _float2* vPos, _uint* iTagNum);
public:
	virtual CGameObject* Clone(void* pArg);
	static CToolUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, COMINDEX* pIndex);
public:
	_bool getClick() { return m_bClicked; }
	_fmatrix getMatirx() { return m_pTransform->getWorldMatrix(); }
	_tchar* getUITexutreName() { return m_pTexture->getTextureName(); }
	UITAG getUITag() { return m_eTag; }
public:
	void setClick(_bool bClicked) { m_bClicked = bClicked; }
	void setTag(UITAG eTag) { m_eTag = eTag; }
private:
	virtual void Free();
private:
	CVIBuffer_Rect* m_pBuffer;
	CTexture* m_pTexture;
	_bool m_bClicked;
	UITAG m_eTag;
};
END
#endif