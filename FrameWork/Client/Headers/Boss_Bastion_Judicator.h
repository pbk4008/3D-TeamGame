#pragma once

#include "Actor.h"

BEGIN(Engine)
class CModel;
class CAnimator;
END

BEGIN(Client)
class CBoss_Bastion_Judicator final : public CActor
{
private:
	explicit CBoss_Bastion_Judicator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Bastion_Judicator(const CBoss_Bastion_Judicator& rhs);
	virtual ~CBoss_Bastion_Judicator() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT SetUp_Components();
	HRESULT Set_Animation_FSM();

private:
	CModel* m_pModelCom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CCapsuleCollider* m_pCollider = nullptr;
	
	class CShieldBreaker* m_pWeapon = nullptr;

private:
	_uint itest = 0;
public:
	static CBoss_Bastion_Judicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
