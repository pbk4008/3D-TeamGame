#pragma once
#ifndef __Effect_FloatingUp_H__
#define __Effect_FloatingUp_H__

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Respawn_Up;
class CTexture;
END

BEGIN(Client)
class CEffect_FloatingUp final : public CEffect
{
public:
	typedef struct tagEffFloatingUpDesc : CEffect::EFFECTDESC
	{
		_float4 ParticleColor;
		_float	Power;
		_float	fRespawnPosY; //�ٽ� ��������y��
		_bool	bSmall; //�������۾������ϰ������
	}EF_PAR_FLOATUP_DESC;
protected:
	explicit CEffect_FloatingUp();
	explicit CEffect_FloatingUp(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_FloatingUp(const CEffect_FloatingUp& rhs);
	virtual ~CEffect_FloatingUp() = default;
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
	EF_PAR_FLOATUP_DESC Get_EffectDesc() {return m_Desc; }

private:
	CVIBuffer_PointInstance_Respawn_Up* m_pBuffer = nullptr;
	class CCullingBox* m_pBox = nullptr;

private:
	EF_PAR_FLOATUP_DESC m_Desc;
	CVIBuffer_PointInstance_Respawn_Up::PIDESC m_backupDesc;

public:
	static CEffect_FloatingUp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif