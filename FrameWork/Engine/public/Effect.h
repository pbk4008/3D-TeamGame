#pragma once
#ifndef __EFFEFCT_H__
#define __EFFEFCT_H__
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class ENGINE_DLL CEffect abstract : public CGameObject
{
	public:
	typedef struct tagEffectDesc
	{
		_tchar		EffectName[MAX_PATH]; //파일이름
		_tchar		ShaderFilePath[MAX_PATH]; //파일경로
		_tchar		ShaderFileName[MAX_PATH]; //파일이름
		_tchar		ShaderFullFilePath[MAX_PATH]; //파일경로 + 파일이름
		_vector		fMyPos; //내 위치
		_matrix		ParticleMat;
		_float3		fParticleRandomPos;
		_float3		fParticleMinusRandomDir; //반대방향으로갈수있게해주는랜덤값
		_float3		fParticleRandomDir;
		_float2		fParticleSize;
		_float		fParticleVelocity;
		_float		fMaxLifeTime;
		_float		fCurTime; //현재나이
		_uint		iNumInstance;
		_uint		iAxis;
		_bool		bUsingGravity;

		//이미지관련
		_tchar		TextureTag[MAX_PATH]; //텍스쳐태그
		_uint		iImageCountX; 
		_uint		iImageCountY;
		_float		fEffectPlaySpeed;
		_float		fFrame;
		_uint		iRenderPassNum;

		_int		IDTag; //고유태그값

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
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	virtual CEffect* Copy();
protected:
	_fvector UsingGravity(_fvector vPos, _double dDeltaTime);
	
public:
	virtual void Set_Reset(_bool bReset);

protected:
	CTexture* m_pTexture;
	EFFECTDESC	m_Desc;

	_float m_fNonActiveTimeAcc = 0.f;

	_bool m_bReset = false; //이펙트 초기화 할 변수 

protected:
	virtual void Free() override;
};
END
#endif