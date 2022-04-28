#pragma once
#ifndef __STARGAZER_H__
#define __STARGAZER_H__

#include "Weapon.h"
BEGIN(Engine)
class CCapsuleCollider;
END
BEGIN(Client)
class CTrailEffect_Distortion;
class CStargazer final : public CWeapon
{
private:
	explicit CStargazer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CStargazer(const CStargazer& _rhs);
	virtual ~CStargazer() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;
	virtual void setActive(_bool bActive) override;
public:
	//��ü�� �Ǻ� ��Ʈ��Ʈ �޾ƿ���
	virtual void Set_OwnerPivotMatrix(const _fmatrix& _smatPivot);
private:
	//������Ʈ �纻 ����
	HRESULT SetUp_Component();
	//������ ����� ����
	_int Attach_FixedBone(const _double& _dDeltaTime);
	//������ ����Ŀ� ���� �������ִ� ��ü�� ����Matrix ���
	_int Attach_Owner(const _double& _dDeltaTime);
	//Material����
	HRESULT SetUp_Material();
	void Check_Attack();
private:
	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual void OnTriggerExit(CCollision& collision) override;
public:
	static CStargazer* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg);
private:
	virtual void Free() override;
private:
	CCapsuleCollider* m_pCollider;
	CTrailEffect_Distortion* m_pTrailEffect_Distortion = nullptr;
};
END
#endif