#pragma once

#include "VIBuffer.h"
#include "SaveManager.h"
BEGIN(Engine)
class CHierarchyNode;
class CMeshContainer final : public CVIBuffer
{
private:
	explicit CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer() = default;
public:
	HRESULT NativeConstruct_Prototype(class CModel* pModel, aiMesh* pMesh, _fmatrix PivotMatirx);
	HRESULT NativeConstruct_Prototype(_uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, class CModel* pModel, void* pVtx, void* pIdx);
	HRESULT NativeConstruct_Prototype(_uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, _uint iType, void* pVtx, void* pIdx);
	HRESULT NativeConstruct(void* pArg) override;
	HRESULT Render();
	HRESULT Render(ID3D11Buffer* pInstVetex, _uint iInstStride);
public:
	HRESULT Create_VertexIndexBuffer();
	HRESULT Add_Bone(class CModel* pModel);
	void SetUp_BoneMatrices(_matrix* pBoneMatrices, _fmatrix PivotMatrix);
	const CSaveManager::STATICMESHDATA& SetStaticSaveData();
	const CSaveManager::ANIMMESHDATA& SetAnimSaveData();
private:
	HRESULT Set_UpVerticesDesc(class CModel* pModel, aiMesh* pMesh, _fmatrix PivotMatrix);
	HRESULT Set_UpVerticesDesc(_uint iNumVtxCnt, class CModel* pModel, void* pVtx);
	HRESULT Set_UpVerticesDesc(_uint iNumVtxCnt, _uint pModelType, void* pVtx);
	HRESULT Set_IndicesDesc(aiMesh* pMesh);
	HRESULT Set_IndicesDesc(_uint iNumIdxCnt, void* pIdx);

	HRESULT SetUp_SkinnedDesc(CModel* pModel, aiMesh* pMesh);
public:
	vector<_uint>* getMaterialIndex() { return &m_vecMaterialIndex; }
public:
	void* getIndices(void) {
		return m_pIndices;
	}
	void* getVertices(void) {
		return m_pVertices;
	}

private:
	aiMesh*					m_pAIMesh = nullptr;
private:
	vector<_uint> m_vecMaterialIndex;
	_uint m_iNumMesh = 0;
private:
	vector<CHierarchyNode*>			m_Bones;
	typedef vector<CHierarchyNode*>	BONES;
	
public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, class CModel* pModel, aiMesh* pMesh, _fmatrix PivotMatrix);
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, class CModel* pModel, void* pVtx, void* pIdx);
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, _uint iType, void* pVtx, void* pIdx);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;
};

END