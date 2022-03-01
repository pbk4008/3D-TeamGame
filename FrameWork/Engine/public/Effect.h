#pragma once
#ifndef __EFFEFCT_H__
#define __EFFEFCT_H__
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CEffect abstract : public CGameObject
{
	public:
	typedef struct tagEffectDesc
	{
		_tchar		TextureTag[MAX_PATH];
		_tchar		ShaderFilePath[MAX_PATH];
		_float3		fPos;
		_float3		fVelocity;
		_float3		fRandom;
		_float2		fParticleSize;
		_float		fMaxLifTime;
		_uint		iNumInstance;
		_bool		bUsingGravity;
	}EFFECTDESC;
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
	CTexture* m_pTexture;
	EFFECTDESC	m_Desc;
protected:
	virtual void Free() override;
};
END
#endif