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
public:
	const _float Get_CurrentHpRatio();
protected:
	HRESULT Set_SpawnPosition(const _float3 vPoint);
public:
	const _bool Get_Dead() { return m_bDead; }
	_float Get_CurrentHp() { return m_fCurrentHp; }
	void Set_CurrentHp(_float fCurrentHp) { m_fCurrentHp = fCurrentHp; }
	const _bool IsAttack() { return m_IsAttack; }
public:
	virtual void Set_IsAttack(const _bool _isAttack) { m_IsAttack = _isAttack; }
protected:
	virtual void Free() override;
protected:
	_float m_fSpeed;//이동 속도
	_bool m_bDead;//죽었는지 안죽었는지 판단
	_bool m_IsAttack;//공격했는지 공격 안했는지
	_float m_fMaxHp;//최대 체력
	_float m_fCurrentHp;//현재 체력
};
END
#endif