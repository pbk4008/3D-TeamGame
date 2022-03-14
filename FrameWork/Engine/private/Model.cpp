#include "Model.h"
#include "MeshContainer.h"
#include "HierarchyNode.h"
#include "Texture.h"
#include "Channel.h"
#include "Animation.h"
#include "Component_Manager.h"
#include "TextureManager.h"
#include "Material.h"
#include "GameInstance.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
	ZeroMemory(&m_tAnimData, sizeof(m_tAnimData));
}

CModel::CModel(const CModel& rhs)
	: CComponent(rhs)
	, m_pScene(rhs.m_pScene)
	, m_Materials(rhs.m_Materials)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_iCurrentAnimation(rhs.m_iCurrentAnimation)
	, m_eMeshType(rhs.m_eMeshType)
	, m_HierarchyNodes(rhs.m_HierarchyNodes)
	, m_bSaved(rhs.m_bSaved)
	, m_bUsingMaterial(rhs.m_bUsingMaterial)
	, m_pEffect(rhs.m_pEffect)
	, m_PassDesc(rhs.m_PassDesc)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_tAnimData(rhs.m_tAnimData)
{	
	strcpy_s(m_szMeshFilePath, rhs.m_szMeshFilePath);
	strcpy_s(m_szMeshFullName, rhs.m_szMeshFullName);

	if (!m_bUsingMaterial)
	{
		for (auto& pMaterial : m_Materials)
		{
			for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
				Safe_AddRef(pMaterial->pMeshTexture[i]);
		}
		m_MeshContainers.resize((_uint)rhs.m_Materials.size());

	}
	else
	{
		m_vecMaterials.resize(rhs.m_vecMaterials.size());
		for (_uint i = 0; i < m_vecMaterials.size(); ++i)
		{
			m_vecMaterials.resize((_uint)rhs.m_vecMaterials.size());
			for (_uint i = 0; i < m_vecMaterials.size(); ++i)
			{
				if (rhs.m_vecMaterials[i])
					Add_Material(rhs.m_vecMaterials[i], i);
			}
			m_MeshContainers.resize((_uint)rhs.m_vecMaterials.size());
		}
	}

	for (auto& MtrlMeshContainer : rhs.m_MeshContainers)
	{
		for (auto& pPrototypeMeshContainer : MtrlMeshContainer)
		{
			CMeshContainer* pMeshContainer = (CMeshContainer*)pPrototypeMeshContainer->Clone(nullptr);
			if (nullptr == pMeshContainer)
				return;

			m_MeshContainers[pMeshContainer->getMaterialIndex()].push_back(pMeshContainer);
		}
	}

	for (auto& pAnimation : rhs.m_Animations)
	{
		m_Animations.emplace_back(pAnimation->Clone());
	}

	for (auto& pPassDesc : m_PassDesc)
	{
		Safe_AddRef(pPassDesc->pInputLayout);
		Safe_AddRef(pPassDesc->pPass);
	}
	Safe_AddRef(m_pEffect);
}

CHierarchyNode* CModel::Get_BoneMatrix(const char * pBoneName)
{
	return Find_HierarchyNode(pBoneName);	
}

CAnimation* CModel::Get_Animation(const string& pName)
{
	CAnimation* pFindAnim = nullptr;
	for (auto& pAnim : m_Animations)
	{
		if (!strcmp(pAnim->Get_Name(), pName.c_str()))
			pFindAnim = pAnim;
	}
	return pFindAnim;
}

HRESULT CModel::NativeConstruct_Prototype(const string& pMeshFilePath, const string& pMeshFileName, const wstring& pShaderFilePath, _fmatrix PivotMatrix, TYPE eMeshType, _bool bUsingMaterial)
{
	m_bUsingMaterial = bUsingMaterial;

	m_eMeshType = eMeshType;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	strcpy_s(m_szMeshFilePath, pMeshFilePath.c_str());

	string szFullPath = pMeshFilePath;
	szFullPath += pMeshFileName;
	strcpy_s(m_szMeshFullName, szFullPath.c_str());

	_int iFlag = 0;

	if(m_eMeshType == CModel::TYPE_STATIC)
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;

 	m_pScene  = m_Importer.ReadFile(szFullPath, iFlag);
 	if (nullptr == m_pScene)
		return E_FAIL;

	if (!m_bUsingMaterial)
		m_MeshContainers.resize(m_pScene->mNumMaterials);

	if (FAILED(Create_MeshContainer()))
		return E_FAIL;

	if (FAILED(Create_VertexIndexBuffer()))
		return E_FAIL;

	if (!m_bUsingMaterial)
	{
		if (FAILED(Create_MaterialDesc()))
			return E_FAIL;
		if (FAILED(Compile_Shader(pShaderFilePath)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Create_Materials()))
			return E_FAIL;
	}

	if (FAILED(Create_Animation()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::NativeConstruct_Prototype(const wstring& pMeshFilePath, TYPE eType, _bool bUsingMaterial)
{
	m_bSaved = true;
	m_bUsingMaterial = bUsingMaterial;
	m_eMeshType = eType;
	if (eType == TYPE_STATIC)
	{
		if (FAILED(Load_StaticModel(pMeshFilePath)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Load_AnimModel(pMeshFilePath)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	if (TYPE_STATIC == m_eMeshType)
		return S_OK;
	if (m_bSaved)
	{
		if (FAILED(Load_HierarchyNode()))
			return E_FAIL;

		_uint iMeshCnt = 0;
		for (auto& MtrlMeshContainers : m_MeshContainers)
		{
			for (auto& pMeshContainer : MtrlMeshContainers)
			{
				pMeshContainer->Add_Bone(m_tAnimData.pMeshData[iMeshCnt].vecBoneName, this);
				iMeshCnt++;
			}
		}
		for (auto& pHierarchyNode : m_HierarchyNodes)
			pHierarchyNode->Update_CombinedTransformationMatrix();

		_uint iNumAnimation = m_tAnimData.iAnimCount;
		for (_uint i = 0; i < iNumAnimation; i++)
		{
			vector<class CChannel*>& pChannels = m_Animations[i]->Get_Channels();

			for (auto& pChannel : pChannels)
			{
				CHierarchyNode* pHierarchyNode = Find_HierarchyNode(pChannel->Get_Name());
				if (!pHierarchyNode)
					return E_FAIL;
				pHierarchyNode->Add_Channel(i, pChannel);
			}
		}


		return S_OK;
	}

	if (FAILED(Create_HierarchyNode(m_pScene->mRootNode)))
		return E_FAIL;

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	for (auto& MtrlMeshContainers : m_MeshContainers)
	{
		for (auto& pMeshContainer : MtrlMeshContainers)
			pMeshContainer->Add_Bone(this);
	}
	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Update_CombinedTransformationMatrix();
	}
	_uint iNumAnimation = m_pScene->mNumAnimations;
	for (_uint i = 0; i < iNumAnimation; i++)
	{
		vector<class CChannel*>& pChannels = m_Animations[i]->Get_Channels();

		for (auto& pChannel : pChannels)
		{
			CHierarchyNode* pHierarchyNode = Find_HierarchyNode(pChannel->Get_Name());
			if (!pHierarchyNode)
				return E_FAIL;
			pHierarchyNode->Add_Channel(i, pChannel);
		}
	}
	return S_OK;
}

HRESULT CModel::Add_Material(CMaterial* _pMtrl, const _uint _iMtrlIndex)
{
	if (_iMtrlIndex < m_vecMaterials.size())
	{
		if (m_vecMaterials[_iMtrlIndex])
			Safe_Release(m_vecMaterials[_iMtrlIndex]);

		m_vecMaterials[_iMtrlIndex] = _pMtrl;
		Safe_AddRef(_pMtrl);

		return S_OK;
	}
	return E_FAIL;
}

HRESULT CModel::SetUp_ValueOnShader(const char* pConstantName, void* pData, _uint iSize)
{
	if (m_bUsingMaterial)
	{
		for (auto& pMtrl : m_vecMaterials)
		{
			if (pMtrl)
				pMtrl->SetUp_ValueOnShader(pConstantName, pData, iSize);
		}
	}
	else
	{
		if (!m_pEffect)
			return E_FAIL;

		ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
		if (!pVariable)
			return E_FAIL;

		return pVariable->SetRawValue(pData, 0, iSize);
	}
	return S_OK;
}

HRESULT CModel::SetUp_TextureOnShader(const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eType)
{
	if (!m_pEffect)
		return E_FAIL;

	if (nullptr == m_Materials[iMeshContainerIndex]->pMeshTexture[eType])
		return S_OK;

	ID3DX11EffectShaderResourceVariable * pVariable = m_pEffect->GetVariableByName(pConstantName)->AsShaderResource();
	if (!pVariable)
		return E_FAIL;

	return pVariable->SetResource(m_Materials[iMeshContainerIndex]->pMeshTexture[eType]->Get_ShaderResourceView());
}

HRESULT CModel::SetUp_TextureOnShader(const char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	ID3DX11EffectShaderResourceVariable* pVariable = m_pEffect->GetVariableByName(pConstantName)->AsShaderResource();
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetResource(pSRV);
}

/* 매 프레임마다 호출. */
HRESULT CModel::Update_CombinedTransformationMatrix(_double TimeDelta)
{
	if (m_eMeshType == TYPE::TYPE_STATIC)
		return S_OK;
	/* 현재 애니메이션 재생시간에 따른 뼈들의 TransformationMatrix를 갱신한다. */
 	m_Animations[m_iCurrentAnimation]->Update_TransformationMatrix(TimeDelta, 1);

	/* 렌더링해야할 CombinedTransfromkationMatrix를 만든다. */
	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Update_CombinedTransformationMatrix(m_iCurrentAnimation);
	}

	return S_OK;
}

HRESULT CModel::Update_CombinedTransformationMatrix(const _int _iCurAnimIndex, const _bool _isRootMotion, const ERootOption _eRootOption)
{
	/* 렌더링해야할 CombinedTransfromkationMatrix를 만든다. */
	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Update_CombinedTransformationMatrix(_iCurAnimIndex, _isRootMotion, _eRootOption);
	}

	return S_OK;
}

HRESULT CModel::Render(_uint iMeshContainerIndex, _uint iPassIndex)
{
	for (auto& pMeshContainer : m_MeshContainers[iMeshContainerIndex])
	{
		if(m_bUsingMaterial)
		{
			_uint iMtrlIndex = pMeshContainer->getMaterialIndex();
			if (m_vecMaterials[iMtrlIndex])
			{
				m_vecMaterials[iMtrlIndex]->Set_InputLayout(iPassIndex);

				if (m_eMeshType == TYPE_ANIM)
				{
					_matrix		BoneMatrices[256];
					ZeroMemory(BoneMatrices, sizeof(_matrix) * 256);

					pMeshContainer->SetUp_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

					if (FAILED(m_vecMaterials[iMtrlIndex]->SetUp_ValueOnShader("g_BoneMatrices", BoneMatrices, sizeof(_matrix) * 256)))
						return E_FAIL;
				}
				if(m_bUsingTool)
					m_vecMaterials[iMtrlIndex]->Using_Tool();
				m_vecMaterials[iMtrlIndex]->Render(iPassIndex);
				pMeshContainer->Render();
			}
		}
		else
		{
			if (iPassIndex >= (_uint)m_PassDesc.size())
				return E_FAIL;
			if (m_eMeshType == TYPE_ANIM)
			{
				_matrix		BoneMatrices[256];
				ZeroMemory(BoneMatrices, sizeof(_matrix) * 256);

				pMeshContainer->SetUp_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

				if (FAILED(SetUp_ValueOnShader("g_BoneMatrices", BoneMatrices, sizeof(_matrix) * 256)))
					return E_FAIL;
			}
			m_pDeviceContext->IASetInputLayout(m_PassDesc[iPassIndex]->pInputLayout);
			m_PassDesc[iPassIndex]->pPass->Apply(0, m_pDeviceContext);

			pMeshContainer->Render();
		}
	}

	return S_OK;
}

HRESULT CModel::Save_Model(const wstring& pFilePath)
{
	if (m_eMeshType == CModel::TYPE_STATIC)
	{
		if (FAILED(Save_StaticModel(pFilePath)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Save_AnimModel(pFilePath)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Load_Animation()
{
	_uint iNumAnimations = m_tAnimData.iAnimCount;

	for (_uint i = 0; i < iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(m_tAnimData.pAnimData[i].szAnimName, m_tAnimData.pAnimData[i].dDuration, m_tAnimData.pAnimData[i].dPlaySpeed, m_tAnimData.pAnimData[i].iAnimIndex);
		if (nullptr == pAnimation)
			return E_FAIL;

		_uint iMaxKeyFrameIndex = 0;

		/* 현재 애니메이션 영햐을 주는 뼈대들 */
		_uint iNumChannels = m_tAnimData.pAnimData[i].iChannelCnt;
		for (_uint j = 0; j < iNumChannels; ++j)
		{
			CChannel* pChannel = CChannel::Create(m_tAnimData.pAnimData[i].pChannelData[j].szChannelName);
			if (nullptr == pChannel)
				return E_FAIL;

			_uint iKeyFrameCnt = m_tAnimData.pAnimData[i].pChannelData[j].iKeyFrameCnt;
			iMaxKeyFrameIndex = iKeyFrameCnt;
			for (_uint k = 0; k < iKeyFrameCnt; ++k)
			{
				KEYFRAME* pKeyFrame = &m_tAnimData.pAnimData[i].pChannelData[j].pKeyFrame[k];
				pChannel->Add_KeyFrame(pKeyFrame);
			}
			pAnimation->Add_Channel(pChannel);
		}
		pAnimation->Set_MaxKeyFrameIndex(iMaxKeyFrameIndex);
		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

HRESULT CModel::Create_Materials()
{
	for (_uint i = 0; i < m_iNumMeshes; i++)
	{
		CMaterial* pMaterial = nullptr;

		if (m_eMeshType == TYPE_STATIC)
			pMaterial = g_pGameInstance->Get_Material();
		else
			pMaterial = g_pGameInstance->Get_Material(L"Default_Anim");

		if (!pMaterial)
			return E_FAIL;
		Safe_AddRef(pMaterial);

		m_vecMaterials.emplace_back(pMaterial);
	}
	return S_OK;
}

HRESULT CModel::Create_MaterialDesc()
{
	if (nullptr == m_pScene)
		return E_FAIL;

	m_Materials.reserve(m_pScene->mNumMaterials);

	char		szMeshFilePath[MAX_PATH] = "";

	for (_uint i = 0; i < m_pScene->mNumMaterials; ++i)
	{
		aiMaterial* pMaterial = m_pScene->mMaterials[i];

		MESHMATERIAL* pMeshMaterial = new MESHMATERIAL;
		ZeroMemory(pMeshMaterial, sizeof(MESHMATERIAL));

		for (_uint j = aiTextureType_DIFFUSE; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString	strFilePath;

			if (FAILED(pMaterial->GetTexture(aiTextureType(j), 0, &strFilePath)))
				continue;

			char	szFileName[MAX_PATH] = "";
			char	szExt[MAX_PATH] = "";
			_splitpath_s(strFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szMeshFilePath, m_szMeshFilePath);
			strcat_s(szMeshFilePath, szFileName);
			strcat_s(szMeshFilePath, szExt);

			wstring     strTexture;
			_tchar		szFullName[MAX_PATH] = TEXT("");
			_tchar		szTextureTag[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szMeshFilePath, (_int)strlen(szMeshFilePath), szFullName, MAX_PATH);
			MultiByteToWideChar(CP_ACP, 0, szFileName, (_int)strlen(szFileName), szTextureTag, MAX_PATH);

			CTextureManager* pTextureMgr = GET_INSTANCE(CTextureManager);

    			pTextureMgr->Add_Texture(m_pDevice, szTextureTag, szFullName);

			RELEASE_INSTANCE(CTextureManager);

			CComponent_Manager* pInstance = GET_INSTANCE(CComponent_Manager);

			strTexture = szTextureTag;
			pMeshMaterial->pMeshTexture[j] = static_cast<CTexture*>(pInstance->Clone_Component(0, L"Texture", &strTexture));

			//wstring wstrSaveFolder = L"../bin/Resource/ "
			lstrcpy(pMeshMaterial->pMeshTextureName[j], szFullName);
			//lstrcpy(pMeshMaterial->pMeshTextureName[j], strTexture.c_str());
			RELEASE_INSTANCE(CComponent_Manager);


			if (nullptr == pMeshMaterial->pMeshTexture[j])
				return E_FAIL;
		}

		m_Materials.push_back(pMeshMaterial);
	}
	return S_OK;
}

HRESULT CModel::Load_Materials(_uint iType, const wstring& pFilePath)
{
	MESHMATERIAL* pDesc = new MESHMATERIAL;

	ZeroMemory(pDesc, sizeof(MESHMATERIAL));

	char szFilePath[MAX_PATH] = "";
	string tmpFilePath;
	tmpFilePath.assign(pFilePath.begin(), pFilePath.end());
	strcpy_s(szFilePath, tmpFilePath.c_str());

	char	szFileName[MAX_PATH] = "";
	char	szExt[MAX_PATH] = "";

	_splitpath_s(szFilePath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	wstring     strTexture;
	_tchar		szFullName[MAX_PATH] = TEXT("");
	_tchar		szTextureTag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, szFilePath, (_int)strlen(szFilePath), szFullName, MAX_PATH);
	MultiByteToWideChar(CP_ACP, 0, szFileName, (_int)strlen(szFileName), szTextureTag, MAX_PATH);

	CTextureManager* pTextureMgr = GET_INSTANCE(CTextureManager);

	pTextureMgr->Add_Texture(m_pDevice, szTextureTag, szFullName);

	RELEASE_INSTANCE(CTextureManager);

	CComponent_Manager* pInstance = GET_INSTANCE(CComponent_Manager);

	strTexture = szTextureTag;
	
	pDesc->pMeshTexture[iType] = static_cast<CTexture*>(pInstance->Clone_Component(0, L"Texture", &strTexture));
	lstrcpy(pDesc->pMeshTextureName[iType], szFullName);
	RELEASE_INSTANCE(CComponent_Manager);

	if (nullptr == pDesc->pMeshTexture[iType])
		return E_FAIL;


	return S_OK;
}

HRESULT CModel::Create_MeshContainer()
{
	if (nullptr == m_pScene)
		return E_FAIL;

	m_iNumMeshes = m_pScene->mNumMeshes;
	if (m_bUsingMaterial)
		m_MeshContainers.resize(m_iNumMeshes);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		aiMesh* pMesh = m_pScene->mMeshes[i];
		if (!pMesh)
			return E_FAIL;

		CMeshContainer* pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, this, pMesh, (m_eMeshType == TYPE_STATIC) ? XMLoadFloat4x4(&m_PivotMatrix) : XMMatrixIdentity());
		if (!pMeshContainer)
			return E_FAIL;
		pMeshContainer->setMeshIndex(i);

		if(!m_bUsingMaterial)
			m_MeshContainers[pMesh->mMaterialIndex].emplace_back(pMeshContainer);
		else
			m_MeshContainers[i].emplace_back(pMeshContainer);
	}
	
	return S_OK;
}

HRESULT CModel::Load_MeshContainer (_uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, void* pVtx, void* pIdx)
{
	CMeshContainer* pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, iMaterialIndex, iNumVtxCnt, iNumIdxCnt,this, pVtx, pIdx);
	if (!pMeshContainer)
		return E_FAIL;

	m_MeshContainers[iMaterialIndex].emplace_back(pMeshContainer);

	return S_OK;
}

HRESULT CModel::Create_VertexIndexBuffer()
{
	for (auto& pMtrlMeshContainer : m_MeshContainers)
	{
		for (auto& pMeshContainer : pMtrlMeshContainer)
			pMeshContainer->Create_VertexIndexBuffer();
	}

	return S_OK;
}

HRESULT CModel::Create_HierarchyNode(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth)
{
	_matrix			TransformationMatrix;
	memcpy(&TransformationMatrix, &pNode->mTransformation, sizeof(_matrix));

	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pNode->mName.data, XMMatrixTranspose(TransformationMatrix), iDepth, pParent);
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	pHierarchyNode->Reserve_Channel(m_pScene->mNumAnimations);

	m_HierarchyNodes.push_back(pHierarchyNode);

	for (_uint i = 0; i < pNode->mNumChildren; ++i)
		Create_HierarchyNode(pNode->mChildren[i], pHierarchyNode, iDepth + 1);

	return S_OK;
}

HRESULT CModel::Load_HierarchyNode()
{
	//CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(pName, matTransform, iDepth, pParent);
	//if (nullptr == pHierarchyNode)
	//	return E_FAIL;

	//pHierarchyNode->Reserve_Channel(m_tAnimData.iAnimCount);

	//m_HierarchyNodes.push_back(pHierarchyNode);

	//for (_uint i = 0; i < pNode->mNumChildren; ++i)
	//	Create_HierarchyNode(pNode->mChildren[i], pHierarchyNode, iDepth + 1);
	for (auto& pNode : m_tAnimData.pBoneData)
	{
		CHierarchyNode* pParent = Find_HierarchyNode(pNode.szParentName);

		CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(pNode.szBoneName, XMLoadFloat4x4(&pNode.OffsetMatrix), XMLoadFloat4x4(&pNode.TransformationMatrix), pNode.iDepth, pParent);
		if (!pHierarchyNode)
			return E_FAIL;

		pHierarchyNode->Reserve_Channel(m_tAnimData.iAnimCount);
		m_HierarchyNodes.emplace_back(pHierarchyNode);
	}

	return S_OK;
}

HRESULT CModel::Create_Animation()
{
	if (m_eMeshType == TYPE_STATIC)
		return S_OK;

	for (_uint i = 0; i < m_pScene->mNumAnimations; ++i)
	{
		aiAnimation*	pAnim = m_pScene->mAnimations[i];

		CAnimation*		pAnimation = CAnimation::Create(pAnim->mName.data, pAnim->mDuration, pAnim->mTicksPerSecond, i);
		if (nullptr == pAnimation)
			return E_FAIL;

		_uint iMaxKeyFrameIndex = 0;

		/* 현재 애니메이션 영햐을 주는 뼈대들 */
		for (_uint j = 0; j < pAnim->mNumChannels; ++j)
		{
			aiNodeAnim*		pAnimChannel = pAnim->mChannels[j];

			CChannel*		pChannel = CChannel::Create(pAnimChannel->mNodeName.data);
			if (nullptr == pChannel)
				return E_FAIL;

			_uint iNumKeyframes = max(pAnimChannel->mNumPositionKeys, pAnimChannel->mNumRotationKeys);
			iNumKeyframes = max(iNumKeyframes, pAnimChannel->mNumScalingKeys);

			if (iNumKeyframes > iMaxKeyFrameIndex)
				iMaxKeyFrameIndex = iNumKeyframes;

			_float3 vScale = _float3(1.f, 1.f, 1.f);
			_float4 vRotation = _float4(0.f, 0.f, 0.f, 0.f);
			_float3 vPosition = _float3(0.f, 0.f, 0.f);
			for (_uint k = 0; k < iNumKeyframes; ++k)
			{		
				KEYFRAME*			pKeyFrame = new KEYFRAME;
				ZeroMemory(pKeyFrame, sizeof(KEYFRAME));

				if (pAnimChannel->mNumScalingKeys > k)
				{
					memcpy(&vScale, &pAnimChannel->mScalingKeys[k].mValue, sizeof(_float3));
					pKeyFrame->Time = pAnimChannel->mScalingKeys[k].mTime;
				}

				if (pAnimChannel->mNumRotationKeys > k)
				{
					vRotation.x = pAnimChannel->mRotationKeys[k].mValue.x;
					vRotation.y = pAnimChannel->mRotationKeys[k].mValue.y;
					vRotation.z = pAnimChannel->mRotationKeys[k].mValue.z;
					vRotation.w = pAnimChannel->mRotationKeys[k].mValue.w;
					pKeyFrame->Time = pAnimChannel->mRotationKeys[k].mTime;
				}

				if (pAnimChannel->mNumPositionKeys > k)
				{
					memcpy(&vPosition, &pAnimChannel->mPositionKeys[k].mValue, sizeof(_float3));
					pKeyFrame->Time = pAnimChannel->mPositionKeys[k].mTime;
				}

				memcpy(&pKeyFrame->vScale, &vScale, sizeof(_float3));
				memcpy(&pKeyFrame->vRotation, &vRotation, sizeof(_float4));
				memcpy(&pKeyFrame->vPosition, &vPosition, sizeof(_float3));

				pChannel->Add_KeyFrame(pKeyFrame);
			}
			pAnimation->Add_Channel(pChannel);
		}
		pAnimation->Set_MaxKeyFrameIndex(iMaxKeyFrameIndex);
		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}
HRESULT CModel::Compile_Shader(const wstring& pShaderFilePath)
{
	_uint iNumElements = 0;

	D3D11_INPUT_ELEMENT_DESC Elements[D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT];
	ZeroMemory(Elements, sizeof(D3D11_INPUT_ELEMENT_DESC) * D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT);

	if (m_eMeshType == TYPE_STATIC)
	{
		iNumElements = 5;
		Elements[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[4] = { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	}
	else
	{
		iNumElements = 7;
		Elements[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[6] = { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[4] = { "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[5] = { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	}

	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif // _DEBUG	
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);

	D3DX11_TECHNIQUE_DESC			TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	pTechnique->GetDesc(&TechniqueDesc);

	m_PassDesc.reserve(TechniqueDesc.Passes);

	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		EFFECTDESC* pPassDesc = new EFFECTDESC;

		pPassDesc->pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC		PassDesc;
		ZeroMemory(&PassDesc, sizeof(D3DX11_PASS_DESC));

		pPassDesc->pPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(Elements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pPassDesc->pInputLayout)))
			return E_FAIL;

		m_PassDesc.push_back(pPassDesc);
	}

	return S_OK;
}
CHierarchyNode * CModel::Find_HierarchyNode(const char * pName)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CHierarchyNode* pNode)
	{
		return !strcmp(pNode->Get_Name(), pName);
	});

	if (iter == m_HierarchyNodes.end())
		return nullptr;

	return (*iter);
}

HRESULT CModel::Save_StaticModel(const wstring& pFilePath)
{
	CSaveManager* pInstance = GET_INSTANCE(CSaveManager);

	vector<CSaveManager::STATICMESHDATA> vecMesh;
	vector<CSaveManager::MTRLDATA> vecMtrl;

	for(auto& pMtrlMeshContainer : m_MeshContainers)
	{
		for (auto& pMeshContainer : pMtrlMeshContainer)
		{
			CSaveManager::STATICMESHDATA pData;
			ZeroMemory(&pData, sizeof(pData));

			pData = pMeshContainer->SetStaticSaveData();
			vecMesh.emplace_back(pData);
		}
	}

	for (auto& pMaterial : m_vecMaterials)
		vecMtrl.emplace_back(pMaterial->SetMaterialSaveData());

	pInstance->Save_StaticModel(vecMtrl, vecMesh, XMLoadFloat4x4(&m_PivotMatrix), pFilePath);

	RELEASE_INSTANCE(CSaveManager);

	return S_OK;
}

HRESULT CModel::Save_AnimModel(const wstring& pFilePath)
{
	CSaveManager* pInstance = GET_INSTANCE(CSaveManager);

	vector<CSaveManager::ANIMMESHDATA> vecMesh;
	vector<CSaveManager::MTRLDATA> vecMtrl;
	vector<CSaveManager::ANIMDATA> vecAnim;
	vector<CSaveManager::BONEDATA> vecBone;

	for (auto& pMtrlMeshContainer : m_MeshContainers)
	{
		for (auto& pMeshContainer : pMtrlMeshContainer)
		{
			CSaveManager::ANIMMESHDATA pData;
			ZeroMemory(&pData, sizeof(pData));

			pData = pMeshContainer->SetAnimSaveData();
			vecMesh.emplace_back(pData);
		}
	}

	for (auto& pMaterial : m_vecMaterials)
		vecMtrl.emplace_back(pMaterial->SetMaterialSaveData());

	for (auto& pAnim : m_Animations)
	{
		CSaveManager::ANIMDATA pAnimData;
		ZeroMemory(&pAnimData, sizeof(pAnimData));

		pAnimData = pAnim->SetSaveAnimData();

		vecAnim.emplace_back(pAnimData);
	}

	for (auto& pNode : m_HierarchyNodes)
	{
		CSaveManager::BONEDATA pBoneData;
		ZeroMemory(&pBoneData, sizeof(pBoneData));

		strcpy_s(pBoneData.szBoneName, pNode->Get_Name());

		const CHierarchyNode* pParent = pNode->Get_Parent();
		if (pParent)
			strcpy_s(pBoneData.szParentName, pNode->Get_Parent()->Get_Name());
		else
			strcpy_s(pBoneData.szParentName, "");

		pBoneData.iDepth = pNode->Get_Depth();
		XMStoreFloat4x4(&pBoneData.OffsetMatrix, pNode->Get_OffsetMatrix());
		XMStoreFloat4x4(&pBoneData.TransformationMatrix, pNode->Get_TransformMatrix());

		vecBone.emplace_back(pBoneData);
	}


	if (FAILED(pInstance->Save_AnimModel(vecMtrl, vecMesh, vecAnim, vecBone,XMLoadFloat4x4(&m_PivotMatrix), pFilePath)))
		return E_FAIL;

	RELEASE_INSTANCE(CSaveManager);

	return S_OK;
}

HRESULT CModel::Load_StaticModel(const wstring& pFilePath)
{
	CSaveManager* pInstance = GET_INSTANCE(CSaveManager);

	CSaveManager::STATICDATA pData;
	ZeroMemory(&pData, sizeof(pData));
	if (FAILED(pInstance->Load_StaticModel(pData, pFilePath)))
		return E_FAIL;

	RELEASE_INSTANCE(CSaveManager);

	XMStoreFloat4x4(&m_PivotMatrix,XMMatrixIdentity());

	m_iNumMeshes = pData.iMeshCount;
	m_MeshContainers.resize(pData.iMtrlCount);
	for (auto& pMesh : pData.pMeshData)
	{
		if (FAILED(Load_MeshContainer(pMesh.iMeshMtrlNum,pMesh.iVtxCount,pMesh.iIdxCount,pMesh.pVtxPoint, pMesh.pIndex)))
			return E_FAIL;
	}

	if (FAILED(Create_VertexIndexBuffer()))
		return E_FAIL;
	m_vecMaterials.resize(pData.iMtrlCount);
	_uint iMtrlIndex = 0;
	for (auto& pMtrl : pData.pMtrlData)
	{
		CMaterial* pMaterial = g_pGameInstance->Get_Material(pMtrl.pMtrlName);
		if (!pMaterial)
		{
			pMaterial = CMaterial::Create(m_pDevice, m_pDeviceContext, pMtrl.pMtrlName, pMtrl.pShader_Path, (CMaterial::EType)pMtrl.iMtrlType);
			if (!pMaterial)
				return E_FAIL;

			for (auto& pTextureData : pMtrl.vecTextureData)
			{
				wstring wstrTexturePath = L"../../Client/bin/FBX/Texture/";
				wstrTexturePath += pTextureData.pTextureName;
				if (FAILED(pMaterial->Set_Texture((TEXTURETYPE)pTextureData.iType, pTextureData.pTextureName, wstrTexturePath)))
					return E_FAIL;
			}
		}
		g_pGameInstance->Add_Material(pMtrl.pMtrlName, pMaterial);
		Add_Material(pMaterial, iMtrlIndex);
		iMtrlIndex++;
	}
	return S_OK;
}

HRESULT CModel::Load_AnimModel(const wstring& pFilePath)
{
	CSaveManager* pInstance = GET_INSTANCE(CSaveManager);

	_matrix matPivot = XMMatrixIdentity();
	if(FAILED(pInstance->Load_AnimModel(m_tAnimData,matPivot,pFilePath)))
		return E_FAIL;

	RELEASE_INSTANCE(CSaveManager);

	XMStoreFloat4x4(&m_PivotMatrix, matPivot);

	m_iNumMeshes = m_tAnimData.iMeshCount;
	m_MeshContainers.resize(m_tAnimData.iMtrlCount);
	for (auto& pMesh : m_tAnimData.pMeshData)
	{
		if (FAILED(Load_MeshContainer(pMesh.iMeshMtrlNum, pMesh.iVtxCount, pMesh.iIdxCount, pMesh.pVtxPoint, pMesh.pIndex)))
			return E_FAIL;
	}

	if (FAILED(Create_VertexIndexBuffer()))
		return E_FAIL;
	m_vecMaterials.resize(m_tAnimData.iMtrlCount);
	_uint iMtrlIndex = 0;

	for (auto& pMtrl : m_tAnimData.pMtrlData)
	{
		CMaterial* pMaterial = g_pGameInstance->Get_Material(pMtrl.pMtrlName);
		if (!pMaterial)
		{
			pMaterial = CMaterial::Create(m_pDevice, m_pDeviceContext, pMtrl.pMtrlName, pMtrl.pShader_Path, (CMaterial::EType)pMtrl.iMtrlType);
			if (!pMaterial)
				return E_FAIL;

			for (auto& pTextureData : pMtrl.vecTextureData)
			{
				wstring wstrTexturePath = L"../../Client/bin/FBX/Texture/";
				wstrTexturePath += pTextureData.pTextureName;
				if (FAILED(pMaterial->Set_Texture((TEXTURETYPE)pTextureData.iType, pTextureData.pTextureName, wstrTexturePath)))
					return E_FAIL;
			}
		}
		g_pGameInstance->Add_Material(pMtrl.pMtrlName, pMaterial);
		Add_Material(pMaterial, iMtrlIndex);
		iMtrlIndex++;
	}

	if (FAILED(Load_Animation()))
		return E_FAIL;

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const string& pMeshFilePath, const string& pMeshFileName, const wstring& pShaderFilePath, _fmatrix PivotMatrix, TYPE eMeshType, _bool bMaterial)
{
	CModel*		pInstance = new CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pMeshFilePath, pMeshFileName, pShaderFilePath, PivotMatrix, eMeshType,bMaterial)))
	{
		MSGBOX("Failed to Creating CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pMeshFileName, TYPE eType ,_bool bUsingMaterial)
{
	CModel* pInstance = new CModel(pDevice, pDeviceContext);
	if (pInstance->NativeConstruct_Prototype(pMeshFileName, eType,bUsingMaterial))
	{
		MSGBOX("CModel Load Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*		pInstance = new CModel(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		for (auto& pPassDec : m_PassDesc)
		{
			Safe_Release(pPassDec->pInputLayout);
			Safe_Release(pPassDec->pPass);
		}
		m_PassDesc.clear();
	}
	Safe_Release(m_pEffect);

	for (auto& pNode : m_HierarchyNodes)
		Safe_Release(pNode);

	m_HierarchyNodes.clear();

	for (auto& pMtrl : m_vecMaterials)
	{
		Safe_Release(pMtrl);
	}
	m_vecMaterials.clear();

	for (auto& pMaterial : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(pMaterial->pMeshTexture[i]);
		if (false == m_isCloned)
			Safe_Delete(pMaterial);
	}
	m_Materials.clear();

	for (auto& pMtrlMeshContainer : m_MeshContainers)
	{
		for (auto& pMeshContainer : pMtrlMeshContainer)
			Safe_Release(pMeshContainer);
		pMtrlMeshContainer.clear();
	}
	m_MeshContainers.clear();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	m_Animations.clear();

	m_Importer.FreeScene();
}
