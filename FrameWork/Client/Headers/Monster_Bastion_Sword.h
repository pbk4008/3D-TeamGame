#pragma once

#include "Actor.h"

BEGIN(Engine)
class CModel;
class CAnimator;
END

BEGIN(Client)
class CShieldBreaker;
class CStargazer;
class CMonster_Bastion_Sword final : public CActor
{
private:
	enum class ANIM_TYPE { HEAD, IDLE, ATTACK, RUN_START, RUN_END, RUN_LOOP, HIT, TYPE_END};
private:
	explicit CMonster_Bastion_Sword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_Bastion_Sword(const CMonster_Bastion_Sword& _rhs);
	virtual ~CMonster_Bastion_Sword() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT SetUp_Components();
	HRESULT Set_Animation_FSM();
	HRESULT Set_Weapon();

private:
	CModel* m_pModelCom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CCapsuleCollider* m_pCollider = nullptr;
private:
	//CShieldBreaker* m_pWeapon = nullptr;
	CStargazer* m_pWeapon = nullptr;
public:
	static CMonster_Bastion_Sword* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};
END
