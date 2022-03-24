#pragma once
#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include"SingleTon.h"
BEGIN(Client)
class CMeshLoader final : public CSingleTon<CMeshLoader>
{
	friend CSingleTon;
public:
	typedef struct MESHTYPE
	{
		_tchar* szFBXPath;
		_tchar* szFBXName;
		_uint iType;
		_uint iLoaderIndex = 0;
	}MESHTYPE;
private:
	explicit CMeshLoader();
	virtual ~CMeshLoader() = default;
public:
	HRESULT Add_MeshLoader(MESHTYPE* tType, _bool& bCheck);
	HRESULT Reserve_MeshLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void Update();
	_bool Get_AllWorking();
public:
	void Resume_Thread();
	void Wait_AllThread();
public:
	_bool Get_Clear() { return m_bClear; };
private:
	static _uint CALLBACK Load_FBX(void* pArg);
private:
	virtual void Free() override;
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	HANDLE m_HandleArr[10];
	vector<_bool> m_vecWorking;
	CRITICAL_SECTION m_Critical;
	_bool m_bClear;
};
END
#endif