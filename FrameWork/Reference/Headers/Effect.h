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
		_vector		fMyPos; //�� ��ġ
		_matrix		ParticleMat;
		_float3		fParticleRandomPos;
		_float3		fParticleRandomDir;
		_float2		fParticleSize;
		_float		fParticleVelocity;
		_float		fMaxLifeTime;
		_uint		iNumInstance;
		_uint		iAxis;
		_bool		bUsingGravity;
	}EFFECTDESC;
protected:
	explicit CEffect();
	explicit CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;
protected:
	/* ������ü�� �����ɶ� ȣ��Ǵ� �Լ�. */
	virtual HRESULT NativeConstruct_Prototype();
	/* ��������ü�� �����ɶ� ȣ��Ǵ� �Լ�. */
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