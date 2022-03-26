#pragma once
#ifndef __EFFECTMANAGER_H__
#define __EFFECTMANAGER_H__

#include "SingleTon.h"
BEGIN(Engine)
class CEffect;
class ENGINE_DLL CEffectManager final : public CSingleTon<CEffectManager>
{
	friend CSingleTon;
private:
	NO_COPY(CEffectManager);
	explicit CEffectManager();
	virtual ~CEffectManager()=default;
public:
	HRESULT Ready_EffectManager(ID3D11Device * pDevcie, ID3D11DeviceContext * pDeviceContext);
	//이펙트 추가
	HRESULT Add_Effect(_uint iSceneID, const wstring& pLayerTag, CEffect* pEffect, _uint iCount);
	//이펙트 가져오기
	CEffect* Get_Effect(_uint iEffectIndex);
private:
	CEffect* Find_Effect(_uint iEffectIndex);
private:
	virtual void Free() override;
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
private:
	vector<list<CEffect*>> m_vecEffect;
};
END

#endif