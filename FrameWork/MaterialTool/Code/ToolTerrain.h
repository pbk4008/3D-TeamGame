#pragma once
#ifndef __TOOL_TERRAIN_H__
#define __TOOL_TERRAIN_H__
#include "GameObject.h"


BEGIN(Tool)
class CToolCell;
class CToolTerrain final : public CGameObject
{
public:
	typedef struct tagMouseDesc
	{
		_float3 vPos;
		_float fRadius;
	}MOUSEDESC;
private:
	explicit CToolTerrain();
	explicit CToolTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CToolTerrain() = default;
public:
	HRESULT Init_ToolTerrain();
	_int Update_Terrain(_float fDeltaTime);
	virtual HRESULT Render_GameObject() override;
public:
	virtual CGameObject* Clone(void* pArg) override { return nullptr; };
	static CToolTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	HRESULT Draw_HeightMap();
	HRESULT Draw_SameHeight();
public:
	void setMouseDesc(MOUSEDESC pDesc, _int iHitIndex);
	void Check_Draw(_uint iCheck);
	HRESULT Change_TerrainTexture(const _tchar* pTextureTag);
	HRESULT Save_HeightMap(const _tchar* pFilePath);
	HRESULT Load_HeighMap(const _tchar* pFilePath);
	HRESULT Add_Cell(CToolCell* pCell);
	HRESULT Delete_Cell(_uint iIndex);
	HRESULT Change_CellPos(_uint iIndex, _fvector vPos);
private:
	virtual void Free() override;
public:
	void* getVertices() { return m_pBuffer->getVertices(); }
	_uint getVtxX() { return m_pBuffer->getVerteicesX(); }
	_uint getVtxZ() { return m_pBuffer->getVerteicesZ(); }
	_fmatrix getWorldMatrx() { return m_pTransform->getWorldMatrix(); }
public:
	void setSameHeight(_bool bHeight) { m_bSameHeight = bHeight; }
	void setUsingNaviTool(_bool bUsing) { m_bUsingNaviTool = bUsing; }
private:
	_bool m_bUsingNaviTool;
	CVIBuffer_Terrain* m_pBuffer;
	CTexture* m_pSrcTexture;
	vector<CTexture*> m_vecTexture;
	_uint m_iTextureCount;
	MOUSEDESC m_tMouseDesc;
	_bool m_bSameHeight;
	_int m_iHitIndex;
	_uint m_iDrawCheck;
private:
	CNaviMesh* m_pNaviMesh;
};
END

#endif