#pragma once

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Floating_Disappear;
class CTexture;
class CCullingBox;

END
class CMFCEffect_Floating_Disappear final : public CEffect
{
protected:
	explicit CMFCEffect_Floating_Disappear();
	explicit CMFCEffect_Floating_Disappear(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMFCEffect_Floating_Disappear(const CEffect& rhs);
	virtual ~CMFCEffect_Floating_Disappear() = default;
protected:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype();
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT SetUp_Components();

public:
	CEffect::EFFECTDESC Get_EffectDesc() {return m_Desc; }

public:
	virtual CEffect* Copy() override;

private:
	CVIBuffer_PointInstance_Floating_Disappear* m_pBuffer = nullptr;
	CCullingBox* m_pBox = nullptr;

private:
	CEffect::EFFECTDESC m_Desc;
	_bool m_bReset = false;
	_float m_fAlpha = 1.f;
	_float m_fDisappearTimeAcc = 0.f;
	_bool m_bShow = true;
public:
	static CMFCEffect_Floating_Disappear* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg) override;
	virtual void Free() override;
};

