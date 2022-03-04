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

	for(auto&pMtrl : vecMtrlData)
	{
		_uint iTextureCnt = pMtrl.iTextureCnt;
		WriteFile(hFile, &iTextureCnt, sizeof(_uint), &dwByte, nullptr);

		for(auto& pTexture : pMtrl.pTaxtureData)
		{
			WriteFile(hFile, &pTexture.iType, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &pTexture.iTextureNameSize, sizeof(_uint), &dwByte, nullptr);
			_uint iTextureNameSize = pTexture.iTextureNameSize;
			WriteFile(hFile, pTexture.pTextureName, sizeof(_tchar) * iTextureNameSize, &dwByte, nullptr);
		}
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

HRESULT CSaveManager::Load_StaticModel(STATICDATA& StaticData, const wstring& pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	_ulong dwByte = 1;

	ZeroMemory(&StaticData, sizeof(STATICDATA));

	ReadFile(hFile, &StaticData.iMtrlCount, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &StaticData.iMeshCount, sizeof(_uint), &dwByte, nullptr);

	StaticData.pMtrlData.reserve(StaticData.iMtrlCount);
	for (_uint i = 0; i < StaticData.iMtrlCount; i++)
	{
		MTRLDATA pMTrl;
		ZeroMemory(&pMTrl, sizeof(pMTrl));
		ReadFile(hFile, &pMTrl.iTextureCnt, sizeof(_uint), &dwByte, nullptr);

		pMTrl.pTaxtureData.reserve(pMTrl.iTextureCnt);
		for (_uint j = 0; j < pMTrl.iTextureCnt; j++)
		{
			TEXTUREDATA pTexture;
			ZeroMemory(&pTexture, sizeof(pTexture));
			ReadFile(hFile, &pTexture.iType, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &pTexture.iTextureNameSize, sizeof(_uint), &dwByte, nullptr);
			_uint iTextureNameSize = pTexture.iTextureNameSize;
			ReadFile(hFile, pTexture.pTextureName, sizeof(_tchar) * iTextureNameSize, &dwByte, nullptr);
			
			pMTrl.pTaxtureData.emplace_back(pTexture);
		}
		StaticData.pMtrlData.emplace_back(pMTrl);
	}
	StaticData.pMeshData.reserve(StaticData.iMeshCount);
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
