#pragma once
#ifndef __Effect_Falling_Leaf_H__
#define __Effect_Falling_Leaf_H__

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Respawn;
class CTexture;
END

BEGIN(Client)
class CEffect_Falling_Leaf final : public CEffect
{
protected:
	explicit CEffect_Falling_Leaf();
	explicit CEffect_Falling_Leaf(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Falling_Leaf(const CEffect_Falling_Leaf& rhs);
	virtual ~CEffect_Falling_Leaf() = default;
protected:
	/* ������ü�� �����ɶ� ȣ��Ǵ� �Լ�. */
	virtual HRESULT NativeConstruct_Prototype();
	/* ��������ü�� �����ɶ� ȣ��Ǵ� �Լ�. */
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
	CVIBuffer_PointInstance_Respawn* m_pBuffer = nullptr;
	class CCullingBox* m_pBox = nullptr;

private:
	CEffect::EFFECTDESC m_Desc;
	CVIBuffer_PointInstance_Respawn::PIDESC m_backupDesc;

public:
	static CEffect_Falling_Leaf* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif