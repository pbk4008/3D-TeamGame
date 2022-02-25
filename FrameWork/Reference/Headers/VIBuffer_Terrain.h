#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, const _tchar* pHeightMapPath);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Culling(const wstring& pCameraTag, _fmatrix WorldMatrixInverse);
public:
	void* getVertices() { return m_pVertices; }
	_uint getVerteicesX() { return m_iNumVerticesX; }
	_uint getVerteicesZ() { return m_iNumVerticesZ; }
private:
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;

private:
	class CQuadTree*		m_pQuadTree = nullptr;

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath, const _tchar* pHeightMapPath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END