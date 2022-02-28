#pragma once

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_PointInstance;
class CTexture;
END
class CMFCEffect final : public CEffect
{
public:
	typedef struct tagEffectDesc
	{
		_tchar TextureTag[MAX_PATH];
		_float4 fPos;
		_float3 fVelocity;
	}EFFECTDESC;
protected:
	explicit CMFCEffect();
	explicit CMFCEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMFCEffect(const CEffect& rhs);
	virtual ~CMFCEffect() = default;
protected:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype();
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CVIBuffer_PointInstance* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;

private:
	EFFECTDESC m_Desc;

public:
	static CMFCEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

