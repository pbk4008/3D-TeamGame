#ifndef __Effect_HitFloating_H__
#define __Effect_HitFloating_H__

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance_Floating;
class CTexture;
END

BEGIN(Client)
class CEffect_HitFloating final : public CEffect
{
protected:
	explicit CEffect_HitFloating();
	explicit CEffect_HitFloating(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_HitFloating(const CEffect_HitFloating& rhs);
	virtual ~CEffect_HitFloating() = default;
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

private:
	virtual HRESULT SetUp_Components();

public:
	CEffect::EFFECTDESC Get_EffectDesc() {return m_Desc; }

private:
	CVIBuffer_PointInstance_Floating* m_pBuffer = nullptr;

private:
	CEffect::EFFECTDESC m_Desc;
	CVIBuffer_PointInstance_Floating::PIDESC m_backupDesc;

	_float m_fNonActiveTimeAcc = 0.f;

public:
	static CEffect_HitFloating* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif