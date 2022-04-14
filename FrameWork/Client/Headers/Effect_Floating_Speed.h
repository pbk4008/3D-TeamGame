#ifndef __Effect_Floating_Speed_H__
#define __Effect_Floating_Speed_H__

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Floating_Speed;
class CTexture;
END

BEGIN(Client)
class CEffect_Floating_Speed final : public CEffect
{
public:
	typedef struct tagEffFloatingSpeedDesc : CEffect::EFFECTDESC
	{
		_float4 ParticleColor;
		_float	Power;
	}FLOATINGSPEEDDESC;
	
protected:
	explicit CEffect_Floating_Speed();
	explicit CEffect_Floating_Speed(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Floating_Speed(const CEffect_Floating_Speed& rhs);
	virtual ~CEffect_Floating_Speed() = default;
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
private:
	virtual CEffect* Copy() override;
	virtual void Set_Reset(_bool bReset);

public:
	CEffect::EFFECTDESC Get_EffectDesc() {return m_Desc; }
private:
	CVIBuffer_PointInstance_Floating_Speed* m_pBuffer = nullptr;
	class CCullingBox* m_pBox = nullptr;

private:
	FLOATINGSPEEDDESC m_Desc;
	CVIBuffer_PointInstance_Floating_Speed::PIDESC m_backupDesc;

public:
	static CEffect_Floating_Speed* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif