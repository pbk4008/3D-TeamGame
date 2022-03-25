#pragma once
#ifndef __SHOOTER_CHASE_H__
#define __SHOOTER_CHASE_H__

#include "Monster_FSM.h"

BEGIN(Client)

class CShooter_Chase final : public CMonster_FSM
{
private:
	explicit CShooter_Chase(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CShooter_Chase() = default;
public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();
public:
	//���� ����� ȣ���ϴ� �̺�Ʈ(�� ���·� �ٲܶ�)
	virtual HRESULT EnterState();
	//���� ����� ȣ���ϴ� �̺�Ʈ(�ٸ� ���·� �ٲ� ��)
	virtual HRESULT ExitState();
	virtual HRESULT EnterState(void* pArg);
	virtual HRESULT ExitState(void* pArg);
public:
	static CShooter_Chase* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
private:
	virtual void Free() override;
private:
	_float m_fAngle;
};
END
#endif