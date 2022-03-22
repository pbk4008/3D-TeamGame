#pragma once
#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include"SingleTon.h"
BEGIN(Client)
class CMeshLoader final : public CSingleTon<CMeshLoader>
{
	friend CSingleTon;
public:
	typedef struct tagMeshLoader
	{
		HANDLE hThread;
		_bool bWorking = false;
	}MESHLOADER;
	typedef struct MESHTYPE
	{
		_tchar szFBXPath[MAX_PATH];
		_tchar szFBXName[MAX_PATH];
		_uint iType;
		_uint iLoaderIndex = 0;
	}MESHTYPE;
private:
	explicit CMeshLoader();
	virtual ~CMeshLoader() = default;
public:
	HRESULT Add_MeshLoader(MESHTYPE& tType);
	HRESULT Reserve_MeshLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	_bool Get_AllWorking();
private:
	static _uint CALLBACK Load_FBX(void* pArg);
private:
	virtual void Free() override;
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	vector<MESHLOADER> m_vecMeshLoader;
	CRITICAL_SECTION m_Critical;
	_bool m_bPause;
};
END
#endif