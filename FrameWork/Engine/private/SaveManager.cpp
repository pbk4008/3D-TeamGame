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

HRESULT CSaveManager::Save_AnimModel(vector<MTRLDATA>& vecMtrlData, vector <ANIMMESHDATA>& vecMeshData, vector<ANIMDATA>& vecAnimData, vector<BONEDATA>& vecBoneData,_fmatrix pivotMatirx, const wstring& pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	_ulong dwByte = 0;
	_uint iMtrlSize = (_uint)vecMtrlData.size();
	_uint iMeshSize = (_uint)vecMeshData.size();
	_uint iAnimSize = (_uint)vecAnimData.size();
	_uint iBoneSize = (_uint)vecBoneData.size();

	WriteFile(hFile, &iMtrlSize, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &iMeshSize, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &iAnimSize, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &iBoneSize, sizeof(_uint), &dwByte, nullptr);

	WriteFile(hFile, &pivotMatirx, sizeof(_matrix), &dwByte, nullptr);

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
		_uint iBoneCount = pMesh.iBoneCnt;
		WriteFile(hFile, &pMesh.iVtxCount, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &pMesh.iIdxCount, sizeof(_uint), &dwByte, nullptr);
		
		WriteFile(hFile, pMesh.pVtxPoint, sizeof(VTXMESH_ANIM) * iVtxCount, &dwByte, nullptr);
		WriteFile(hFile, pMesh.pIndex, sizeof(FACEINDICES32) * iIdxCount, &dwByte, nullptr);
		WriteFile(hFile, &pMesh.iMeshMtrlNum, sizeof(_uint), &dwByte, nullptr);

		WriteFile(hFile, &iBoneCount, sizeof(_uint), &dwByte, nullptr);
		for (auto& pName : pMesh.vecBoneName)
		{
			char tmp[MAX_PATH] = "";
			strcpy_s(tmp, pName.c_str());
			WriteFile(hFile, &tmp, sizeof(char)*MAX_PATH, &dwByte, nullptr);
		}
	}
	for (auto& pAnim : vecAnimData)
	{
		WriteFile(hFile, &pAnim.dDuration, sizeof(_double), &dwByte, nullptr);
		WriteFile(hFile, &pAnim.dPlaySpeed, sizeof(_double), &dwByte, nullptr);

		WriteFile(hFile, &pAnim.szAnimName, sizeof(char)*MAX_PATH, &dwByte, nullptr);
		WriteFile(hFile, &pAnim.iAnimIndex, sizeof(_uint), &dwByte, nullptr);

		_uint iChannelCnt = pAnim.iChannelCnt;
		WriteFile(hFile, &iChannelCnt, sizeof(_uint), &dwByte, nullptr);
		for (_uint i = 0; i < iChannelCnt; i++)
		{
			WriteFile(hFile, &pAnim.pChannelData[i].szChannelName, sizeof(char)*MAX_PATH, &dwByte, nullptr);

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

	for (auto& pBone : vecBoneData)
		WriteFile(hFile, &pBone, sizeof(BONEDATA),&dwByte, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CSaveManager::Load_AnimModel(DYNAMICDATA& AnimData, _matrix& pivotMatrix, const wstring& pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;


	_ulong dwByte = 1;
	ReadFile(hFile, &AnimData.iMtrlCount, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &AnimData.iMeshCount, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &AnimData.iAnimCount, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &AnimData.iBoneCount, sizeof(_uint), &dwByte, nullptr);

	ReadFile(hFile, &pivotMatrix, sizeof(_matrix), &dwByte, nullptr);

	AnimData.pMtrlData.resize(AnimData.iMtrlCount);
	for (_uint i=0; i<AnimData.iMtrlCount; i++)
	{
		MTRLDATA tMtrlData;

		ReadFile(hFile, &tMtrlData.iTextureCnt, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &tMtrlData.iMtrlType, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &tMtrlData.pMtrlName, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
		ReadFile(hFile, &tMtrlData.pShader_Path, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);

		for (_uint j = 0; j < tMtrlData.iTextureCnt; j++)
		{
			TEXTUREDATA tTextureData;
			ZeroMemory(&tTextureData, sizeof(TEXTUREDATA));
			ReadFile(hFile, &tTextureData, sizeof(TEXTUREDATA), &dwByte, nullptr);

			tMtrlData.vecTextureData.emplace_back(tTextureData);
		}
		AnimData.pMtrlData[i]=tMtrlData;
	}

	AnimData.pMeshData.resize(AnimData.iMeshCount);
	for (_uint i=0; i<AnimData.iMeshCount; i++)
	{
		ANIMMESHDATA tAnimMeshData;

		ReadFile(hFile, &tAnimMeshData.iVtxCount, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &tAnimMeshData.iIdxCount, sizeof(_uint), &dwByte, nullptr);

		tAnimMeshData.pVtxPoint = new VTXMESH_ANIM[tAnimMeshData.iVtxCount];
		ZeroMemory(tAnimMeshData.pVtxPoint, sizeof(VTXMESH) * tAnimMeshData.iVtxCount);
		tAnimMeshData.pIndex = new FACEINDICES32[tAnimMeshData.iIdxCount];
		ZeroMemory(tAnimMeshData.pIndex, sizeof(FACEINDICES32) * tAnimMeshData.iIdxCount);

		ReadFile(hFile, tAnimMeshData.pVtxPoint, sizeof(VTXMESH_ANIM) * tAnimMeshData.iVtxCount, &dwByte, nullptr);
		ReadFile(hFile, tAnimMeshData.pIndex, sizeof(FACEINDICES32) * tAnimMeshData.iIdxCount, &dwByte, nullptr);
		ReadFile(hFile, &tAnimMeshData.iMeshMtrlNum, sizeof(_uint), &dwByte, nullptr);

		_uint iBoneCnt = 0;
		ReadFile(hFile, &iBoneCnt, sizeof(_uint), &dwByte, nullptr);
		tAnimMeshData.vecBoneName.resize(iBoneCnt);
		for(_uint j =0; j< iBoneCnt; j++)
		{
			char strBoneName[MAX_PATH] = "";
			ReadFile(hFile, &strBoneName, sizeof(char) * MAX_PATH, &dwByte, nullptr);
			string tmp = strBoneName;
			tAnimMeshData.vecBoneName[j]=tmp;
		}
		AnimData.pMeshData[i] = tAnimMeshData;
	}

	AnimData.pAnimData.resize(AnimData.iAnimCount);
	for (_uint i=0; i<AnimData.iAnimCount; i++)
	{
		ANIMDATA tAnimData;
		ZeroMemory(&tAnimData, sizeof(ANIMDATA));

		ReadFile(hFile, &tAnimData.dDuration, sizeof(_double), &dwByte, nullptr);
		ReadFile(hFile, &tAnimData.dPlaySpeed, sizeof(_double), &dwByte, nullptr);

		ReadFile(hFile, &tAnimData.szAnimName, sizeof(char) * MAX_PATH, &dwByte, nullptr);
		ReadFile(hFile, &tAnimData.iAnimIndex, sizeof(_uint), &dwByte, nullptr);

		ReadFile(hFile, &tAnimData.iChannelCnt, sizeof(_uint), &dwByte, nullptr);

		tAnimData.pChannelData = new CHANNELDATA[tAnimData.iChannelCnt];
		ZeroMemory(tAnimData.pChannelData, sizeof(CHANNELDATA)*tAnimData.iChannelCnt);

		for (_uint j = 0; j < tAnimData.iChannelCnt; j++)
		{
			ReadFile(hFile, &tAnimData.pChannelData[j].szChannelName, sizeof(char) * MAX_PATH, &dwByte, nullptr);
			ReadFile(hFile, &tAnimData.pChannelData[j].iKeyFrameCnt, sizeof(_uint), &dwByte, nullptr);

			tAnimData.pChannelData[j].pKeyFrame = new KEYFRAME[tAnimData.pChannelData[j].iKeyFrameCnt];
			ZeroMemory(tAnimData.pChannelData[j].pKeyFrame, sizeof(KEYFRAME)*tAnimData.pChannelData[j].iKeyFrameCnt);

			for (_uint k= 0; k < tAnimData.pChannelData[j].iKeyFrameCnt; k++)
			{
				ReadFile(hFile, &tAnimData.pChannelData[j].pKeyFrame[k].Time, sizeof(_double), &dwByte, nullptr);
				ReadFile(hFile, &tAnimData.pChannelData[j].pKeyFrame[k].vScale, sizeof(_float3), &dwByte, nullptr);
				ReadFile(hFile, &tAnimData.pChannelData[j].pKeyFrame[k].vRotation, sizeof(_float4), &dwByte, nullptr);
				ReadFile(hFile, &tAnimData.pChannelData[j].pKeyFrame[k].vPosition, sizeof(_float3), &dwByte, nullptr);
			}
		}
		AnimData.pAnimData[i]=tAnimData;
	}

	AnimData.pBoneData.resize(AnimData.iBoneCount);
	for (_uint i = 0; i < AnimData.iBoneCount; i++)
	{
		BONEDATA tBoneData;
		ZeroMemory(&tBoneData, sizeof(BONEDATA));
		ReadFile(hFile, &tBoneData, sizeof(BONEDATA), &dwByte, nullptr);

		AnimData.pBoneData[i] = tBoneData;
	}

	CloseHandle(hFile);

	return S_OK;
}
HRESULT CSaveManager::Load_StaticModel(STATICDATA& StaticData, const wstring& pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	_ulong dwByte = 1;

	ReadFile(hFile, &StaticData.iMtrlCount, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &StaticData.iMeshCount, sizeof(_uint), &dwByte, nullptr);

	StaticData.pMtrlData.resize(StaticData.iMtrlCount);
	for (_uint i = 0; i < StaticData.iMtrlCount; i++)
	{
		MTRLDATA pMTrl;
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
		StaticData.pMtrlData[i] = pMTrl;
	}
	StaticData.pMeshData.resize(StaticData.iMeshCount);
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

		StaticData.pMeshData[i] = pMesh;
	}
	CloseHandle(hFile);

	return S_OK;
}

void CSaveManager::Free()
{
}
