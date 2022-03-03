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
	HRESULT NativeConstruct(void* pArg) override;
	HRESULT Render();
public:
	HRESULT Create_VertexIndexBuffer();
	HRESULT Add_Bone(class CModel* pModel);
	void SetUp_BoneMatrices(_matrix* pBoneMatrices, _fmatrix PivotMatrix);
	const CSaveManager::STATICMESHDATA& SetSaveData();
private:
	HRESULT Set_UpVerticesDesc(class CModel* pModel, aiMesh* pMesh, _fmatrix PivotMatrix);
	HRESULT Set_IndicesDesc(aiMesh* pMesh);
	HRESULT SetUp_SkinnedDesc(class CModel* pModel, aiMesh* pMesh);
public:
	_uint getMaterialIndex() { return m_iMaterialIndex; }
private:
	aiMesh*					m_pAIMesh = nullptr;
private:
	_uint m_iMaterialIndex = 0;
	_uint m_iNumMesh = 0;
private:
	vector<CHierarchyNode*>			m_Bones;
	typedef vector<CHierarchyNode*>	BONES;
	
public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, class CModel* pModel, aiMesh* pMesh, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;
};

END