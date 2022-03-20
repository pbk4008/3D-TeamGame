#pragma once
#ifndef __INTANCING_MEHS_H__
#define __INTANCING_MEHS_H__
#include "VIBuffer.h"
#include "SaveManager.h"
BEGIN(Engine)
class CMeshContainer;
class CMaterial;
class ENGINE_DLL CInstancing_Mesh final : public CComponent
{
public:
enum class INSTANCE_TYPE {STATIC, ANIM, INSTANCE_END};
public:
	typedef struct tagInstancingMeshDesc
	{
		_uint iNumInstance;
		vector<_float4x4> vecMatrix;
	}INSTANCINGMESHDESC;
private:
	explicit CInstancing_Mesh() =default;
	explicit CInstancing_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CInstancing_Mesh(const CInstancing_Mesh& rhs);
	virtual ~CInstancing_Mesh() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const wstring& pMeshFilePath, const wstring& pShaderFile, INSTANCE_TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg) override;
	void Update_InstanceBuffer(const vector<_float4x4>& pMatrix);
public:
	HRESULT SetUp_ValueOnShader(const char* pConstantName, void* pData, _uint iSize);
	HRESULT	SetUp_TextureOnShader(const char* pConstantName, ID3D11ShaderResourceView* pSRV, _ulong Numindex);
	HRESULT Render(_uint iMeshContainerIndex, _int iPassindex);
private:
	HRESULT Init_StaticMesh(const wstring& pMeshFilePath);
	HRESULT Create_MeshContainer(_uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, void* pVtx, void* pIdx);
	HRESULT Create_VertextIndexBuffer();
	HRESULT Create_Material(const CSaveManager::MTRLDATA& pData);
	HRESULT Create_InstancingBuffer(void* pArg);
public:
	static CInstancing_Mesh* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const wstring& pMeshFilePath, const wstring& pShaderFile, INSTANCE_TYPE eType);
	virtual CComponent* Clone(void* pArg) override;

private:
	virtual void Free() override;
public:
	_uint Get_NumMeshContainer() { return (_uint)m_vecMeshContainers.size(); }
private:
	_uint m_iNumMeshes = 0;
private:
	_float4x4 m_matPivot;
	vector<vector<CMeshContainer*>> m_vecMeshContainers;
	vector<CMaterial*> m_vecMaterials;
private:
	ID3D11Buffer* m_pVBInstance = nullptr;
	D3D11_BUFFER_DESC m_VBInstDesc;
	D3D11_SUBRESOURCE_DATA m_VBInstSubResourceData;
	_uint m_iInstStride=0;
	_uint m_iInstNumVertices=0;
private:
	INSTANCE_TYPE m_eType;
};
END
#endif