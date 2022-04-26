#pragma once
#ifndef __Effect_Hammer_Dust_H__
#define __Effect_Hammer_Dust_H__

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEffect_Hammer_Dust final : public CEffect
{
protected:
	explicit CEffect_Hammer_Dust();
	explicit CEffect_Hammer_Dust(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Hammer_Dust(const CEffect& rhs);
	virtual ~CEffect_Hammer_Dust() = default;

public:
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
	CVIBuffer_Rect* m_pBuffer = nullptr;

private:
	CEffect::EFFECTDESC m_Desc;
	_float				m_Alpha = 1.f;

public:
	static CEffect_Hammer_Dust* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif