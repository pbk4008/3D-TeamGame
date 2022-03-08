#pragma once

#include "Actor.h"

BEGIN(Client)

class CMonster_Bastion_Sword : public CActor
{
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
	HRESULT Ready_Components();

private:
	CModel* m_pModel = nullptr;
	CAnimationController* m_pAnimationController = nullptr;

public:
	static CMonster_Bastion_Sword* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END