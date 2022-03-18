#pragma once

#include "Actor.h"

BEGIN(Client)

class CMonster_Bastion_Shooter : public CActor
{
public:
	enum class ANIM_TYPE {
		HEAD, IDLE, ATTACK, RUN_START, RUN_END, RUN_LOOP
		, HIT, TYPE_END
	};

private:
	explicit CMonster_Bastion_Shooter(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_Bastion_Shooter(const CMonster_Bastion_Shooter& _rhs);
	virtual ~CMonster_Bastion_Shooter() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_AnimationFSM();
	HRESULT Ready_StateFSM();

private:
	CModel* m_pModelCom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CStateController* m_pStateController = nullptr;

public:
	static CMonster_Bastion_Shooter* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END