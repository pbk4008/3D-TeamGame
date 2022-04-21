#pragma once
#ifndef __BASTION_SWORD_EXCUTION_H__
#define __BASTION_SWORD_EXCUTION_H__
#include "Monster_FSM.h"
BEGIN(Client)

class CBastion_Sword_Excution final : public CMonster_FSM
{
private:
	explicit CBastion_Sword_Excution(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CBastion_Sword_Excution() = default;
public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();
public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();
	virtual HRESULT EnterState(void* pArg);
	virtual HRESULT ExitState(void* pArg);
public:
	static CBastion_Sword_Excution* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
private:
	virtual void Free() override;
};
END

#endif
