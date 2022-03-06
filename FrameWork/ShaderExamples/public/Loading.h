#pragma once
#ifndef __LOADING_H__
#define __LOADING_H__

#include "Level.h"
#include "Defines.h"

BEGIN(Example)
class CLoader;
class CLoading final : public CLevel
{
private:
	explicit CLoading();
	explicit CLoading(ID3D11Device* pDevice, ID3D11DeviceContext* pDevcieContext);
	virtual~CLoading() = default;
public:
	virtual HRESULT NativeConstruct(SCENEID eID);
	virtual _int Tick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT Move_Scene();
	HRESULT Ready_GameObject();
public:
	static CLoading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevcieContext, SCENEID eID);
private:
	virtual void Free();
private:
	SCENEID m_eSceneID;
	CLoader* m_pLoader;
};
END
#endif