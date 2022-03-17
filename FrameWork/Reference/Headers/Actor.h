#pragma once
#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CActor abstract : public CGameObject
{
protected:
	explicit CActor();
	explicit CActor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CActor(const CActor& rhs);
	virtual ~CActor() = default;
protected:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype();
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
protected:
	virtual void Free() override;
protected:
	_float m_fSpeed;
	_bool m_bDead;
};
END
#endif