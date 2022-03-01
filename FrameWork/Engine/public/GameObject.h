#pragma once

/* Ŭ���̾�Ʈ�� ������ ���ӿ��(��ü, ������Ʈ)���� �θ� �Ǵ� Ŭ������. */
#include "Base.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject();
	explicit CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	class CComponent* Get_Component(const wstring& pComponentTag);
	template<typename T>
	T* Get_Component(const wstring& pComponentTag)
	{
		CComponent* pCom = Get_Component(pComponentTag);

		if (!pCom)
			return nullptr;

		return static_cast<T*>(pCom);
	}
public:
	/* ������ü�� �����ɶ� ȣ��Ǵ� �Լ�. */
	virtual HRESULT NativeConstruct_Prototype();
	/* ��������ü�� �����ɶ� ȣ��Ǵ� �Լ�. */
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	virtual void OnCollisionEnter(CGameObject* pCol);
	virtual void OnCollisionStay(CGameObject* pCol);
	virtual void OnCollisionExit(CGameObject* pCol);
	virtual void OnTriggerEnter(CGameObject* pCol);
	virtual void OnTriggerExit(CGameObject* pCol);
public:
	void setActive(_bool bActive);
	_bool getActive() { return m_bActive; }
protected:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
	CTransform* m_pTransform;
	CRenderer* m_pRenderer;
	_bool m_bActive;
	_bool m_bCheckCollider;
protected:
	unordered_map<wstring, class CComponent*>		m_Components;
protected:
	virtual HRESULT SetUp_Components(_uint iLevelIndex, const wstring& pPrototypeTag, const wstring& pComponentTag, CComponent** ppOut, void* pArg = nullptr);
	virtual HRESULT SetUp_Components(const wstring& pComponentTag, CComponent* pClone);
public:
	virtual CGameObject* Clone(void* pArg) = 0; 
	virtual void Free() override;
};

END