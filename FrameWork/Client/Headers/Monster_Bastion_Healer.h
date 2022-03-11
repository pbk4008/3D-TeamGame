#pragma once


#include "Actor.h"

BEGIN(Client)

class CMonster_Bastion_Healer : public CActor
{
private:
	explicit CMonster_Bastion_Healer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_Bastion_Healer(const CMonster_Bastion_Healer& _rhs);
	virtual ~CMonster_Bastion_Healer() = default;

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
	static CMonster_Bastion_Healer* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END