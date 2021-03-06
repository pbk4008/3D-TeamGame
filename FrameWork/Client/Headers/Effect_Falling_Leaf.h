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
public:
	typedef struct tagEffFloatingLeafDesc : CEffect::EFFECTDESC
	{
		_float	fRespawnPosY; //다시 리스폰될y값
		_bool	bSmall; //사이즈작아지게하고싶을때 
	}FLOATINGLEAFDESC;
protected:
	explicit CEffect_Falling_Leaf();
	explicit CEffect_Falling_Leaf(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Falling_Leaf(const CEffect_Falling_Leaf& rhs);
	virtual ~CEffect_Falling_Leaf() = default;
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
	virtual void Set_Reset(_bool bReset);

private:
	virtual HRESULT SetUp_Components();

public:
	CEffect::EFFECTDESC Get_EffectDesc() {return m_Desc; }

private:
	CVIBuffer_PointInstance_Respawn* m_pBuffer = nullptr;
	class CCullingBox* m_pBox = nullptr;

private:
	FLOATINGLEAFDESC m_Desc;
	CVIBuffer_PointInstance_Respawn::PIDESC m_backupDesc;

public:
	static CEffect_Falling_Leaf* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif