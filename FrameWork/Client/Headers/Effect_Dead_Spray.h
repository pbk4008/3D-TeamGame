#pragma once
#ifndef __Effect_Dead_Spray_H__
#define __Effect_Dead_Spray_H__

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEffect_Dead_Spray final : public CEffect
{
protected:
	explicit CEffect_Dead_Spray();
	explicit CEffect_Dead_Spray(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Dead_Spray(const CEffect& rhs);
	virtual ~CEffect_Dead_Spray() = default;
protected:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype();
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	virtual CEffect* Copy();
private:
	virtual HRESULT SetUp_Components();

public:
	CEffect::EFFECTDESC Get_EffectDesc() {return m_Desc; }

private:
	CVIBuffer_Rect* m_pBuffer = nullptr;

private:
	CEffect::EFFECTDESC m_Desc;
	_float m_fSize = 6.f;
	_float m_fAccRetain=0.f;
public:
	static CEffect_Dead_Spray* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif