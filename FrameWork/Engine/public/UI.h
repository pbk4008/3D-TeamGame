#pragma once
#ifndef __UI_H__
#define __UI_H__
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CVIBuffer_Trapezium;
class ENGINE_DLL CUI abstract : public CGameObject
{
public:
	typedef struct tagUIDesc
	{
		 _tchar TextureTag[MAX_PATH];
		_float3 fPos;
		_float2 fSize;
		_float	fAngle;
		_int	IDTag;
		_bool	bMinus;
	}UIDESC;
protected:
	explicit CUI();
	explicit CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;
protected:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	void Set_Show(_bool bShow) { m_bShow = bShow; }
protected:
	virtual void Free() override;
protected:
	CVIBuffer_Rect* m_pBuffer;
	CVIBuffer_Trapezium* m_pTrapziumBuffer;
	CTexture* m_pTexture;
protected:
	UIDESC m_Desc;
	_bool m_bFirstShow;
	_bool m_bShow;
};
END

#endif