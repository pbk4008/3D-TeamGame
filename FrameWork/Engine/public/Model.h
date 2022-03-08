#pragma once

#include "Component.h"

BEGIN(Engine)
class CTexture;
class CMeshContainer;
class CHierarchyNode;
class CAnimation;
class CMaterial;


class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_STATIC, TYPE_ANIM, TYPE_END };
private:
	explicit CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CModel(const CModel& rhs);
	virtual ~CModel() = default;
public:
	_uint Get_NumMeshContainer() { return (_uint)m_MeshContainers.size(); }
	CHierarchyNode* Get_BoneMatrix(const char* pBoneName);
	_fmatrix Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	TYPE getType() { return m_eMeshType; }
	vector<CHierarchyNode*>& Get_HierachyNodes() { return m_HierarchyNodes; }
	vector<CAnimation*>& Get_Animations() { return m_Animations; }
public:
	HRESULT NativeConstruct_Prototype(const string& pMeshFilePath, const string& pMeshFileName, const wstring& pShaderFilePath, _fmatrix PivotMatrix, TYPE eMeshType,_bool bUsingMaterial);
	HRESULT NativeConstruct_Prototype(const wstring& pMeshFilePath, const wstring& pShaderFilePath, TYPE eType);
	HRESULT NativeConstruct(void* pArg);
public:
	HRESULT Add_Material(CMaterial* _pMtrl, const _uint _iMtrlIndex);

	HRESULT SetUp_ValueOnShader(const char* pConstantName, void* pData, _uint iSize);
	void SetUp_AnimationIndex(_uint iAnimationIndex) { m_iCurrentAnimation = iAnimationIndex; }

	HRESULT Update_CombinedTransformationMatrix(_double TimeDelta);
	HRESULT Update_CombinedTransformationMatrix(const _int _iCurAnimIndex, const _bool _isRootMotion, const ERootOption _eRootOption = ERootOption::XYZ);
	HRESULT Render(_uint iMeshContainerIndex, _uint iPassIndex);
public:
	vector<vector<CMeshContainer*>> Get_MeshContainer() { return m_MeshContainers; }
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
	vector<vector<CMeshContainer*>> m_MeshContainers;
	typedef vector<CMeshContainer*>	MESHCONTAINERS;

	vector<CMaterial*>					m_vecMaterials;

	vector<CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<CHierarchyNode*>	HIERARCHYNODES;

	vector<CAnimation*>				m_Animations;
	typedef vector<CAnimation*>		ANIMATIONS;
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

	HRESULT Create_HierarchyNode(aiNode* pNode, CHierarchyNode* pParent = nullptr, _uint iDepth = 0);
	
	HRESULT Create_Animation();
public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const string& pMeshFilePath, const string& pMeshFileName, const wstring& pShaderFilePath, _fmatrix PivotMatrix, TYPE eMeshType,_bool bMaterial = false);
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pMeshFileName, const wstring& pShaderFilePath, TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END