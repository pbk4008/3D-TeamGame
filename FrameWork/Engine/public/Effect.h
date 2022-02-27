#pragma once
#ifndef __EFFEFCT_H__
#define __EFFEFCT_H__
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CEffect abstract : public CGameObject
{
protected:
	explicit CEffect();
	explicit CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;
protected:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype();
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
protected:
	_fvector UsingGravity(_fvector vPos, _double dDeltaTime);
protected:
	virtual void Free() override;
protected:
	_float m_fMaxLifTime;
	_bool m_bUsingGravity;
	_uint m_iNumEffectCount;
protected:
	
};
END
#endif