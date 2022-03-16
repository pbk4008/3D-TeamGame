#pragma once

#include "Actor.h"
#include "UI_Monster_Panel.h"

BEGIN(Engine)
class CModel;
class CCapsuleCollider;
class CAnimator;
END

BEGIN(Client)
class CMonster_Crawler final : public CActor
{
private:
	explicit CMonster_Crawler(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_Crawler(const CMonster_Crawler& _rhs);
	virtual ~CMonster_Crawler() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT SetUp_Components();
	HRESULT Animation_Setting();

private:
	CModel* m_pModelCom = nullptr;
	//CAnimationController* m_pAnimControllerCom = nullptr;
	CAnimator* m_pAnimatorCom = nullptr;
	CCapsuleCollider* m_pColliderCom = nullptr;

	CUI_Monster_Panel* m_pPanel = nullptr;
private:
	_uint itest = 0;

public:
	static CMonster_Crawler* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};
END
