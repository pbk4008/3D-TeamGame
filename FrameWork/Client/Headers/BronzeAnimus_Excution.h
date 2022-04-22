#pragma once
#ifndef __BRONZEAINMUS_EXCUTION_H__
#define __BRONZEAINMUS_EXCUTION_H__
#include "BronzeAnimus_State.h"
BEGIN(Client)
class CBronzeAnimus_Excution : public CBronzeAnimus_State
{
protected:
	explicit CBronzeAnimus_Excution(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBronzeAnimus_Excution(const CBronzeAnimus_Excution& _rhs);
	virtual ~CBronzeAnimus_Excution() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();
	virtual HRESULT EnterState(void* pArg);
	virtual HRESULT ExitState(void* pArg);

public:
	virtual void Look_Player(void) override;
	virtual void Look_Monster(void) override;

private:
	_bool	m_bAttackEnd = false;

public:
	static CBronzeAnimus_Excution* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};
END
#endif