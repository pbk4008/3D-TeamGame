#ifndef __Effect_Env_Floating_H__
#define __Effect_Env_Floating_H__

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Env_Floating;
class CTexture;
END

BEGIN(Client)
class CEffect_Env_Floating final : public CEffect
{
protected:
	explicit CEffect_Env_Floating();
	explicit CEffect_Env_Floating(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Env_Floating(const CEffect_Env_Floating& rhs);
	virtual ~CEffect_Env_Floating() = default;
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
	CVIBuffer_PointInstance_Env_Floating* m_pBuffer = nullptr;
	class CCullingBox* m_pBox = nullptr;

private:
	CEffect::EFFECTDESC m_Desc;
	CVIBuffer_PointInstance_Env_Floating::PIDESC m_backupDesc;

public:
	static CEffect_Env_Floating* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif