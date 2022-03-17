#pragma once

#include "Actor.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CMonster_BronzeAnimus final : public CActor
{
private:
	explicit CMonster_BronzeAnimus(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_BronzeAnimus(const CMonster_BronzeAnimus& _rhs);
	virtual ~CMonster_BronzeAnimus() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT SetUp_Components();

private:
	CModel* m_pModelCom = nullptr;
	CAnimationController* m_pAnimControllerCom = nullptr;

private:
	_uint itest = 0;

public:
	static CMonster_BronzeAnimus* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};
END
