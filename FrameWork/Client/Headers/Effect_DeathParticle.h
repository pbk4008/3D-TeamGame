#pragma once
#ifndef __Effect_DeathParticle_H__
#define __Effect_DeathParticle_H__

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Explosion;
class CTexture;
END

BEGIN(Client)
class CEffect_DeathParticle final : public CEffect
{
protected:
	explicit CEffect_DeathParticle();
	explicit CEffect_DeathParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_DeathParticle(const CEffect& rhs);
	virtual ~CEffect_DeathParticle() = default;
protected:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype();
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT SetUp_Components();
	virtual CEffect* Copy() override;
	virtual void Set_Reset(_bool bReset);

public:
	CEffect::EFFECTDESC Get_EffectDesc() {return m_Desc; }

private:
	CVIBuffer_PointInstance_Explosion* m_pBuffer = nullptr;
	class CCullingBox* m_pBox = nullptr;

private:
	CEffect::EFFECTDESC m_Desc;
	CVIBuffer_PointInstance_Explosion::PIDESC m_backupDesc;

public:
	static CEffect_DeathParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif