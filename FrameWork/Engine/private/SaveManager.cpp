#include "SaveManager.h"

CSaveManager::CSaveManager()
{
}

HRESULT CSaveManager::Save_StaticModel(vector<MTRLDATA>& vecMtrlData, vector<STATICMESHDATA>& vecMeshData, _fmatrix pivotMatrix,const wstring& pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	_ulong dwByte = 0;

	_uint iMtrlSize = (_uint)vecMtrlData.size();
	_uint iMeshSize = (_uint)vecMeshData.size();
	WriteFile(hFile, &iMtrlSize, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &iMeshSize, sizeof(_uint), &dwByte, nullptr);

	for (auto& pMtrl : vecMtrlData)
	{
		_uint iTextureCnt = pMtrl.iTextureCnt;
		WriteFile(hFile, &iTextureCnt, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &pMtrl.iMtrlType, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &pMtrl.pMtrlName, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
		WriteFile(hFile, &pMtrl.pShader_Path, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);

		for (auto& pTexture : pMtrl.vecTextureData)
			WriteFile(hFile, &pTexture, sizeof(TEXTUREDATA), &dwByte, nullptr);
	}
	for (auto& pMesh : vecMeshData)
	{
		_uint iVtxCount = pMesh.iVtxCount;
		_uint iIdxCount = pMesh.iIdxCount;
		WriteFile(hFile, &pMesh.iVtxCount, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &pMesh.iIdxCount, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, pMesh.pVtxPoint, sizeof(VTXMESH)* iVtxCount, &dwByte, nullptr);
		WriteFile(hFile, pMesh.pIndex, sizeof(FACEINDICES32)* iIdxCount, &dwByte, nullptr);
		WriteFile(hFile, &pMesh.iMeshMtrlNum, sizeof(_uint), &dwByte, nullptr);
	}
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CSaveManager::Save_AnimModel(vector<MTRLDATA>& vecMtrlData, vector <ANIMMESHDATA>& vecMeshData, vector<ANIMDATA>& vecAnimData, _fmatrix pivotMatirx, const wstring& pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	_ulong dwByte = 0;
	_uint iMtrlSize = (_uint)vecMtrlData.size();
	_uint iMeshSize = (_uint)vecMeshData.size();
	_uint iAnimSize = (_uint)vecAnimData.size();
	WriteFile(hFile, &iMtrlSize, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &iMeshSize, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &iAnimSize, sizeof(_uint), &dwByte, nullptr);
	for (auto& pMtrl : vecMtrlData)
	{
		_uint iTextureCnt = pMtrl.iTextureCnt;
		WriteFile(hFile, &iTextureCnt, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &pMtrl.pMtrlName, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
		WriteFile(hFile, &pMtrl.pShader_Path, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
		
		for (auto& pTexture : pMtrl.vecTextureData)
			WriteFile(hFile, &pTexture, sizeof(TEXTUREDATA), &dwByte, nullptr);
	}	
	for (auto& pMesh : vecMeshData)
	{
		_uint iVtxCount = pMesh.iVtxCount;
		_uint iIdxCount = pMesh.iIdxCount;
		_uint iBoneCount = pMesh.iBoneCnt;
		WriteFile(hFile, &pMesh.iVtxCount, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &pMesh.iIdxCount, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &pMesh.iBoneCnt, sizeof(_uint), &dwByte, nullptr);
		
		WriteFile(hFile, pMesh.pVtxPoint, sizeof(VTXMESH) * iVtxCount, &dwByte, nullptr);
		WriteFile(hFile, pMesh.pIndex, sizeof(FACEINDICES32) * iIdxCount, &dwByte, nullptr);
		WriteFile(hFile, &pMesh.iMeshMtrlNum, sizeof(_uint), &dwByte, nullptr);

		for (_uint i = 0; i < iBoneCount; i++)
		{
			_uint iBoneNameSize = pMesh.pBoneData[i].iBoneNameSize;
			_uint iParentNameSize = pMesh.pBoneData[i].iParentNameSize;
			WriteFile(hFile, &iBoneNameSize, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &iParentNameSize, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, pMesh.pBoneData[i].szBoneName, sizeof(char)*iBoneNameSize, &dwByte, nullptr);
			WriteFile(hFile, pMesh.pBoneData[i].szParentName, sizeof(char)* iParentNameSize, &dwByte, nullptr);

			WriteFile(hFile, &pMesh.pBoneData[i].iDepth, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &pMesh.pBoneData[i].OffsetMatrix, sizeof(_float4x4), &dwByte, nullptr);
			WriteFile(hFile, &pMesh.pBoneData[i].TransformationMatrix, sizeof(_float4x4), &dwByte, nullptr);
		}
		Safe_Delete_Array(pMesh.pBoneData);
	}
	for (auto& pAnim : vecAnimData)
	{
		WriteFile(hFile, &pAnim.dDuration, sizeof(_double), &dwByte, nullptr);
		WriteFile(hFile, &pAnim.dPlaySpeed, sizeof(_double), &dwByte, nullptr);

		_uint iAnimNameSize = pAnim.iAnimNameSize;
		WriteFile(hFile, &iAnimNameSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &pAnim.szAnimName, sizeof(char)*iAnimNameSize, &dwByte, nullptr);
		WriteFile(hFile, &pAnim.iAnimIndex, sizeof(_uint), &dwByte, nullptr);

		_uint iChannelCnt = pAnim.iChannelCnt;
		WriteFile(hFile, &iChannelCnt, sizeof(_uint), &dwByte, nullptr);
		for (_uint i = 0; i < iChannelCnt; i++)
		{
			_uint iChannelNameSize = pAnim.pChannelData[i].iChannelNameSize;

			WriteFile(hFile, &iChannelNameSize, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, pAnim.pChannelData[i].szChannelName, sizeof(char)*iChannelNameSize, &dwByte, nullptr);

			_uint iKeyFrameCnt = pAnim.pChannelData[i].iKeyFrameCnt;
			WriteFile(hFile, &iKeyFrameCnt, sizeof(_uint), &dwByte, nullptr);
			for (_uint j = 0; j < iKeyFrameCnt; j++)
			{
				WriteFile(hFile, &pAnim.pChannelData[i].pKeyFrame[j].Time, sizeof(_double), &dwByte, nullptr);
				WriteFile(hFile, &pAnim.pChannelData[i].pKeyFrame[j].vScale, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &pAnim.pChannelData[i].pKeyFrame[j].vRotation, sizeof(_float4), &dwByte, nullptr);
				WriteFile(hFile, &pAnim.pChannelData[i].pKeyFrame[j].vPosition, sizeof(_float3), &dwByte, nullptr);
			}
			Safe_Delete_Array(pAnim.pChannelData[i].pKeyFrame);
		}
		Safe_Delete_Array(pAnim.pChannelData);
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CSaveManager::Load_AnimModel(DYNAMICDATA& AnimData, const wstring& pFilePath)
{
	return S_OK;
}
HRESULT CSaveManager::Load_StaticModel(STATICDATA& StaticData, const wstring& pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	_ulong dwByte = 1;

	ZeroMemory(&StaticData, sizeof(STATICDATA));

	ReadFile(hFile, &StaticData.iMtrlCount, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &StaticData.iMeshCount, sizeof(_uint), &dwByte, nullptr);

	//StaticData.pMtrlData.reserve(StaticData.iMtrlCount);
	for (_uint i = 0; i < StaticData.iMtrlCount; i++)
	{
		MTRLDATA pMTrl;
		ZeroMemory(&pMTrl, sizeof(pMTrl));
		ReadFile(hFile, &pMTrl.iTextureCnt, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pMTrl.iMtrlType, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pMTrl.pMtrlName, sizeof(_tchar)*MAX_PATH, &dwByte, nullptr);
		ReadFile(hFile, &pMTrl.pShader_Path, sizeof(_tchar)*MAX_PATH, &dwByte, nullptr);

		for (_uint i = 0; i < pMTrl.iTextureCnt; i++)
		{
			TEXTUREDATA pTexture;
			ZeroMemory(&pTexture, sizeof(pTexture));
			ReadFile(hFile, &pTexture, sizeof(TEXTUREDATA), &dwByte, nullptr);
			pMTrl.vecTextureData.emplace_back(pTexture);
		}
		StaticData.pMtrlData.emplace_back(pMTrl);
	}
	//StaticData.pMeshData.reserve(StaticData.iMeshCount);
	for (_uint i = 0; i < StaticData.iMeshCount; i++)
	{
		STATICMESHDATA pMesh;
		ReadFile(hFile, &pMesh.iVtxCount, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pMesh.iIdxCount, sizeof(_uint), &dwByte, nullptr);
		_uint iVtxCnt, iIdxCnt;
		iVtxCnt = pMesh.iVtxCount;
		iIdxCnt = pMesh.iIdxCount;
		pMesh.pVtxPoint = new VTXMESH[iVtxCnt];
		ZeroMemory(pMesh.pVtxPoint, sizeof(VTXMESH) * iVtxCnt);
		pMesh.pIndex = new FACEINDICES32[iIdxCnt];
		ZeroMemory(pMesh.pIndex, sizeof(FACEINDICES32) * iIdxCnt);
		ReadFile(hFile, pMesh.pVtxPoint, sizeof(VTXMESH) * iVtxCnt, &dwByte, nullptr);
		ReadFile(hFile, pMesh.pIndex, sizeof(FACEINDICES32) * iIdxCnt, &dwByte, nullptr);
		ReadFile(hFile, &pMesh.iMeshMtrlNum, sizeof(_uint), &dwByte, nullptr);

		StaticData.pMeshData.emplace_back(pMesh);
	}
	CloseHandle(hFile);

	return S_OK;
}

void CSaveManager::Free()
{
}
