#pragma once
#ifndef __TESTOBJ_H__
#define __TESTOBJ_H__

#include "GameObject.h"
BEGIN(Engine)
class CBoxCollider;
END
BEGIN(Client)
class CTestObj final : public CGameObject
{
public:
	typedef struct tagTestDesc
	{
		CBoxCollider::BOXDESC tBoxDesc;
	}TESTDESC;
private:
	explicit CTestObj();
	explicit CTestObj(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTestObj(const CTestObj& rhs);
	virtual ~CTestObj() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	virtual void OnCollisionEnter(CGameObject* pObj) override;
	virtual void OnCollisionExit(CGameObject* pObj) override;
	virtual void OnTriggerEnter(CGameObject* pObj) override;
private:
	void move(_double dDeltaTime);
public:
	static CTestObj* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
private:
	virtual void Free() override;
private:
	CBoxCollider* m_pCollider;
};
END
#endif