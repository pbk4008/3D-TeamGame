#pragma once

#include "Component.h"

BEGIN(Engine)
class CAnimation;
class CHierarchyNode;
class CTexture;
class CAnimation;
class CHierarchyNode;
class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_STATIC, TYPE_ANIM, TYPE_END };
private:
	explicit CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshContainer() {
		return (_uint)m_MeshContainers.size();
	}
	CHierarchyNode* Get_BoneMatrix(const char* pBoneName);
	_fmatrix Get_PivotMatrix() {
		return XMLoadFloat4x4(&m_PivotMatrix);
	}
	TYPE getType() { return m_eMeshType; }
	vector<CHierarchyNode*>& Get_HierachyNodes() { return m_HierarchyNodes; }
	vector<CAnimation*>& Get_Animations() { return m_Animations; }
public:
	HRESULT NativeConstruct_Prototype(const string& pMeshFilePath, const string& pMeshFileName, const wstring& pShaderFilePath, _fmatrix PivotMatrix, TYPE eMeshType);
	HRESULT NativeConstruct_Prototype(const wstring& pMeshFilePath, const wstring& pShaderFilePath, TYPE eType);
	HRESULT NativeConstruct(void* pArg);
public:
	HRESULT SetUp_ValueOnShader(const char* pConstantName, void* pData, _uint iSize);
	HRESULT SetUp_TextureOnShader(const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eType);
	void SetUp_AnimationIndex(_uint iAnimationIndex) {
		m_iCurrentAnimation = iAnimationIndex;
	}
	HRESULT Update_CombinedTransformationMatrix(_double TimeDelta);
	HRESULT Render(_uint iMeshContainerIndex, _uint iPassIndex);
public:
	vector<vector<class CMeshContainer*>> Get_MeshContainer() {
		return m_MeshContainers;
	}
private:
	const aiScene*		m_pScene = nullptr;
	Assimp::Importer	m_Importer;
private:
	char				m_szMeshFilePath[MAX_PATH] = "";
	char				m_szMeshFullName[MAX_PATH] = "";
	_float4x4			m_PivotMatrix;
	TYPE				m_eMeshType = TYPE_END;
	_uint				m_iCurrentAnimation = 0;
	_uint				m_iNumMeshes = 0;
	_bool				m_bSaved = false;
private:
	ID3DX11Effect* m_pEffect = nullptr;
	vector<EFFECTDESC*>	m_PassDesc;
private:
	vector<vector<class CMeshContainer*>> m_MeshContainers;
	typedef vector<vector<class CMeshContainer*>>	MESHCONTAINERS;

	vector<MESHMATERIAL*>				m_Materials;
	typedef vector<MESHMATERIAL*>		MESHMATERIALS;

	vector<class CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<class CHierarchyNode*>	HIERARCHYNODES;

	vector<class CAnimation*>				m_Animations;
	typedef vector<class CAnimation*>		ANIMATIONS;
public:
	CHierarchyNode* Find_HierarchyNode(const char* pName);
	HRESULT Save_StaticModel(const wstring& pFilePath);
	HRESULT Save_AnimModel();
private:
	HRESULT Load_StaticModel(const wstring& pFilePath);
private:
	HRESULT Create_Materials();

	HRESULT Load_Materials(_uint iType, const wstring& pFilePath);
	/* 모델을 구성하는 메시들의 정보를 구성한다. */
	HRESULT Create_MeshContainer();

	HRESULT Load_MeshContainer(_uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, void* pVtx, void* pIdx);

	HRESULT Create_VertexIndexBuffer();

	HRESULT Compile_Shader(const wstring& pShaderFilePath);

	HRESULT Create_HierarchyNode(aiNode* pNode, CHierarchyNode* pParent = nullptr, _uint iDepth = 0);
	
	HRESULT Create_Animation();
public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const string& pMeshFilePath, const string& pMeshFileName, const wstring& pShaderFilePath, _fmatrix PivotMatrix, TYPE eMeshType);
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pMeshFileName, const wstring& pShaderFilePath, TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END