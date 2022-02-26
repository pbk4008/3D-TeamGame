#ifndef VIBuffer_Plane_h__
#define VIBuffer_Plane_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Plane final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Plane(ID3D11Device* _pDevice , ID3D11DeviceContext* _pDeviceContext);
	explicit CVIBuffer_Plane(const CVIBuffer_Plane& _rhs);
	virtual ~CVIBuffer_Plane(void) = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar * _pShaderFilePath, _uint _numVertsX, _uint _numVertsZ);
	virtual HRESULT NativeConstruct(void* _pArg) override;

	/* for. Picking */
public:
	FACEINDICES32*  Get_Indices(void)  { return m_pIndices; }
	VTXTEX*			Get_Vertices(void) { return (VTXTEX*)m_pVertices; }
	_uint			Get_NumFaces(void) { return m_iNumPrimitive; }

public:
	FACEINDICES32* m_pIndices;

public:
	_uint		m_iNumVerticesX = 0;
	_uint		m_iNumVerticesZ = 0;
public:
	void* getVertices() { return m_pVertices; }
	_uint getVerteicesX() { return m_iNumVerticesX; }
	_uint getVerteicesZ() { return m_iNumVerticesZ; }
public:
	static CVIBuffer_Plane* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, const _tchar* _pShaderFilePath, _uint _numVertsX, _uint _numVertsZ);
	virtual CComponent* Clone(void* _pArg)override;
	virtual void Free(void) override;
};

END
#endif //VIBuffer_Plane