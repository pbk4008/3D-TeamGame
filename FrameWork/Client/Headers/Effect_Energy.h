#pragma once
#ifndef __Effect_Energy_H__
#define __Effect_Energy_H__

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Energy;
class CTexture;
END

BEGIN(Client)
class CEffect_Energy final : public CEffect
{
public:
	typedef struct tagEffHitDesc : CEffect::EFFECTDESC
	{
		_float4 ParticleColor;
		_float	Power;
	}EF_PAR_ENERGY_DESC;
protected:
	explicit CEffect_Energy();
	explicit CEffect_Energy(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Energy(const CEffect_Energy& rhs);
	virtual ~CEffect_Energy() = default;
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
	EF_PAR_ENERGY_DESC Get_EffectDesc() {return m_Desc; }

private:
	CVIBuffer_PointInstance_Energy* m_pBuffer = nullptr;
	class CCullingBox* m_pBox = nullptr;

private:
	EF_PAR_ENERGY_DESC m_Desc;
	CVIBuffer_PointInstance_Energy::PIDESC m_backupDesc;

public:
	static CEffect_Energy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif