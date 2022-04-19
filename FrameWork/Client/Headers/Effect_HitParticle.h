#pragma once
#ifndef __Effect_HitParticle_H__
#define __Effect_HitParticle_H__

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Explosion;
class CTexture;
END

BEGIN(Client)
class CEffect_HitParticle final : public CEffect
{
public:
	typedef struct tagEffHitDesc : CEffect::EFFECTDESC
	{
		_float4 ParticleColor;
		_float	Power;
	}EF_PAR_HIT_DESC;
protected:
	explicit CEffect_HitParticle();
	explicit CEffect_HitParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_HitParticle(const CEffect_HitParticle& rhs);
	virtual ~CEffect_HitParticle() = default;
protected:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype();
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	virtual CEffect* Copy() override;
	virtual void Set_Reset(_bool bReset);

private:
	virtual HRESULT SetUp_Components();

public:
	EF_PAR_HIT_DESC Get_EffectDesc() {return m_Desc; }

private:
	CVIBuffer_PointInstance_Explosion* m_pBuffer = nullptr;
	class CCullingBox* m_pBox = nullptr;

private:
	EF_PAR_HIT_DESC m_Desc;
	CVIBuffer_PointInstance_Explosion::PIDESC m_backupDesc;

public:
	static CEffect_HitParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif