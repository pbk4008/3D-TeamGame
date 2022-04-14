#pragma once

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Floating_Speed;
class CTexture;
class CCullingBox;

END
class CMFCEffect_Floating_Speed final : public CEffect
{
protected:
	explicit CMFCEffect_Floating_Speed();
	explicit CMFCEffect_Floating_Speed(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMFCEffect_Floating_Speed(const CEffect& rhs);
	virtual ~CMFCEffect_Floating_Speed() = default;
protected:
	/* ������ü�� �����ɶ� ȣ��Ǵ� �Լ�. */
	virtual HRESULT NativeConstruct_Prototype();
	/* ��������ü�� �����ɶ� ȣ��Ǵ� �Լ�. */
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
	CVIBuffer_PointInstance_Floating_Speed* m_pBuffer = nullptr;
	CCullingBox* m_pBox = nullptr;

private:
	CEffect::EFFECTDESC m_Desc;
	_bool m_bReset = false;
public:
	static CMFCEffect_Floating_Speed* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg) override;
	virtual void Free() override;
};

