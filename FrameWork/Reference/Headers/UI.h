#pragma once
#ifndef __UI_H__
#define __UI_H__
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class ENGINE_DLL CUI abstract : public CGameObject
{
protected:
	explicit CUI();
	explicit CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;
protected:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
protected:
	virtual void Free() override;
protected:
	CVIBuffer_Rect* m_pBuffer;
	CTexture* m_pTexture;
};
END

#endif