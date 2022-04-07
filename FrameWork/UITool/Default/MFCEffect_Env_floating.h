#pragma once

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Env_Floating;
class CTexture;
class CCullingBox;
END
class CMFCEffect_Env_Floating final : public CEffect
{
protected:
	explicit CMFCEffect_Env_Floating();
	explicit CMFCEffect_Env_Floating(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMFCEffect_Env_Floating(const CEffect& rhs);
	virtual ~CMFCEffect_Env_Floating() = default;
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
	CVIBuffer_PointInstance_Env_Floating* m_pBuffer = nullptr;
	CCullingBox* m_pBox = nullptr;

private:
	CEffect::EFFECTDESC m_Desc;
	_bool m_bReset = false;
public:
	static CMFCEffect_Env_Floating* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg) override;
	virtual void Free() override;
};

