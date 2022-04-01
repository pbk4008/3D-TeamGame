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
		_tchar		EffectName[MAX_PATH]; //�����̸�
		_tchar		ShaderFilePath[MAX_PATH]; //���ϰ��
		_tchar		ShaderFileName[MAX_PATH]; //�����̸�
		_tchar		ShaderFullFilePath[MAX_PATH]; //���ϰ�� + �����̸�
		_vector		fMyPos; //�� ��ġ
		_matrix		ParticleMat;
		_float3		fParticleRandomPos;
		_float3		fParticleMinusRandomDir; //�ݴ�������ΰ����ְ����ִ·�����
		_float3		fParticleRandomDir;
		_float2		fParticleSize;
		_float		fParticleVelocity;
		_float		fMaxLifeTime;
		_float		fCurTime; //���糪��
		_uint		iNumInstance;
		_uint		iAxis;
		_bool		bUsingGravity;

		//�̹�������
		_tchar		TextureTag[MAX_PATH]; //�ؽ����±�
		_uint		iImageCountX; 
		_uint		iImageCountY;
		_float		fEffectPlaySpeed;
		_float		fFrame;
		_uint		iRenderPassNum;

		_int		IDTag; //�����±װ�

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

	_bool m_bReset = false; //����Ʈ �ʱ�ȭ �� ���� 

protected:
	virtual void Free() override;
};
END
#endif