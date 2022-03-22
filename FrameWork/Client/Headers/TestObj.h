#pragma once
#ifndef __TESTOBJ_H__
#define __TESTOBJ_H__

#include "GameObject.h"
BEGIN(Engine)
class CBoxCollider;
class CSphreCollider;
class CCapsuleCollider;
END
BEGIN(Client)
class CTestObj final : public CGameObject
{
public:
	typedef struct tagTestDesc
	{
		CBoxCollider::DESC tBoxDesc;
		CSphereCollider::DESC tSphereDesc;
		CCapsuleCollider::DESC tCapsuleDesc;
		ERigidType _eRigidType = ERigidType::Dynamic;
		_float3 vPosition = { 0.f, 0.f, 0.f };
		_bool isMove = false;
		_bool isTrigger = false;
	}TESTDESC;
private:
	explicit CTestObj();
	explicit CTestObj(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTestObj(const CTestObj& rhs);
	virtual ~CTestObj() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	virtual void OnCollisionEnter(CCollision& _collision) override;
	virtual void OnCollisionExit(CCollision& _collision) override;
	virtual void OnTriggerEnter(CCollision& _collision) override;

public:
	void setIsMove(const _bool _isMove);

private:
	void move(_double dDeltaTime);

private:
	_bool m_isMove = false;
	TESTDESC m_tTestDesc;

public:
	static CTestObj* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg);
private:
	virtual void Free() override;
private:
	CBoxCollider* m_pBoxCollider;
	CSphereCollider* m_pSphereCollider;
	CCapsuleCollider* m_pCapsuleCollider;
};
END
#endif