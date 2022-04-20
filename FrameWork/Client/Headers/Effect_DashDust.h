#pragma once
#ifndef __Effect_DashDust_H__
#define __Effect_DashDust_H__

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Floating_Disappear;
class CTexture;
END

BEGIN(Client)
class CEffect_DashDust final : public CEffect
{
protected:
	explicit CEffect_DashDust();
	explicit CEffect_DashDust(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_DashDust(const CEffect& rhs);
	virtual ~CEffect_DashDust() = default;
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
	CEffect::EFFECTDESC Get_EffectDesc() {return m_Desc; }

private:
	CVIBuffer_PointInstance_Floating_Disappear* m_pBuffer = nullptr;
	class CCullingBox* m_pBox = nullptr;

private:
	_float m_fAlpha = 1.f;
	_float m_fDisappearTimeAcc = 0.f;
	_bool m_bShow = true;

private:
	CEffect::EFFECTDESC m_Desc;
	CVIBuffer_PointInstance_Floating_Disappear::PIDESC m_backupDesc;

public:
	static CEffect_DashDust* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif