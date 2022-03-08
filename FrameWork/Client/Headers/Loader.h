#pragma once
#ifndef __LOADER_H__
#define __LOADER_H__
#include "Base.h"
#include "GameObject.h"
BEGIN(Engine)
class CTexutre;
END
BEGIN(Client)
class CLoader final : public CBase
{
private:
	explicit CLoader();
	explicit CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual~CLoader()=default;
public:
	HRESULT Init_Loader(SCENEID eID);
private:
	HRESULT LoadForScene();
private:
	HRESULT SetUp_Stage1_ProtoComponent();
	HRESULT Load_Stage1FBXLoad();
public:
	static _uint CALLBACK Thread_Main(void* pArg);
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, SCENEID eID);
private:
	HRESULT Ready_Logo();
	HRESULT Ready_Stage1();
	HRESULT Ready_Test_JS();

private:
	virtual void Free();
public:
	CRITICAL_SECTION* getCritical() { return &m_Critical; }
	SCENEID getNextScene() { return m_eNextScene; }
	_bool getFinish() { return m_bFinish; }
private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_Critical;
	_bool m_bFinish;
	SCENEID m_eNextScene;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	/*vector<CTexutre*> m_vectexture;*/
};
END
#endif