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
	WriteFile(hFile, &pivotMatrix, sizeof(_float4x4), &dwByte, nullptr);

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

void CSaveManager::Free()
{
}
