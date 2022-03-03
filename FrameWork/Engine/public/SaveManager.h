#pragma once
#ifndef __SAVEMANAGER_H__
#define __SAVEMANAGER_H__
#include "SingleTon.h"
BEGIN(Engine)
class CSaveManager final : public CSingleTon<CSaveManager>
{
friend CSingleTon;
public:
	typedef struct tagStaticMeshData
	{
		_uint iVtxCount;
		_uint iIdxCount;

		VTXMESH* pVtxPoint;
		FACEINDICES32* pIndex;

		_uint iMeshMtrlNum;
	}STATICMESHDATA;
	typedef struct tagTextureData
	{
		_uint iTextureNameSize;
		_tchar* pTextureName;
		_uint iType;
	}TEXTUREDATA;
	typedef struct tagMtrlData
	{
		_uint iTextureCnt;
		vector<TEXTUREDATA> pTaxtureData;
	}MTRLDATA;
	typedef struct tagModelSaveData
	{
		_uint iMeshCount;
		_uint iMtrlCount;
		_float4x4 pivotMatirx;

		MTRLDATA* pMtrlData;
		STATICMESHDATA* pMeshData;
	}STATICDATA;
private:
	NO_COPY(CSaveManager);
	explicit CSaveManager();
	virtual ~CSaveManager() = default;
public:
	template<typename T>
	HRESULT SaveFile(void* pSaveData, const wstring& pFilePath)
	{
		HANDLE hFile = CreateFile(pFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		vector<T> pData = (*(vector<T>*)pSaveData);
		_ulong dwByte = 0;
		for (auto& iter : pData)
		{
			WriteFile(hFile, &iter, sizeof(T), &dwByte, nullptr);
		}
		CloseHandle(hFile);
		return S_OK;
	}
	template<typename T>
	HRESULT LoadFile(vector<T>& pLoadData, const wstring& pFilePath)
	{
		HANDLE hFile = CreateFile(pFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		_ulong dwByte = 1;
		while (true)
		{
			T* pData=new T;
			ReadFile(hFile, pData, sizeof(T), &dwByte, 0);
			if (dwByte == 0)
			{
				Safe_Delete(pData);
				break;
			}
			pLoadData.emplace_back(*pData);
			Safe_Delete(pData);
		}

		CloseHandle(hFile);
		return S_OK;
	}
	HRESULT Save_StaticModel(vector<MTRLDATA>& vecMtrlData, vector<STATICMESHDATA>& vecMeshData, _fmatrix pivotMatrix,const wstring& pFilePath);
	HRESULT Load_StaticModel();
private:
	virtual void Free();
};
END
#endif