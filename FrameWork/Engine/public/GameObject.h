#pragma once

/* Ŭ���̾�Ʈ�� ������ ���ӿ��(��ü, ������Ʈ)���� �θ� �Ǵ� Ŭ������. */
#include "Base.h"
#include "Collision.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CComponent;

#define DESTROY CGameObject::Destroy

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
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
	virtual HRESULT	Render_Shadow();
	virtual HRESULT	Render_Velocity();
	virtual void	ComputeViewZ(_fmatrix* pView);
public:
	virtual void OnCollisionEnter(CCollision& collision);
	virtual void OnCollisionStay(CCollision& collision);
	virtual void OnCollisionExit(CCollision& collision);
	virtual void OnTriggerEnter(CCollision& collision);
	virtual void OnTriggerStay(CCollision& collision); //���ݸ���
	virtual void OnTriggerExit(CCollision& collision);
	virtual void OnControllerColliderHit(CCollision& collision);
public:
	void setActive(_bool bActive);
	_bool getActive() { return m_bActive; }
	_int getTag() { return m_iObectTag; }
	_bool getRemove() { return m_bRemove; }
	CTransform* Get_Transform() const;
	_float		Get_ViewZ() { return m_fViewZ; }
public:
	virtual void Set_Remove(_bool bCheck) { m_bRemove = bCheck; };
	static  void Destroy(CGameObject* _obj);

protected:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
	CTransform* m_pTransform;
	CRenderer* m_pRenderer;
	_bool m_bActive;
	_bool m_bRemove;
	_bool m_bCheckCollider;
	_uint m_iObectTag;
	_uint m_iSceneID;
	_float m_fViewZ = 0.f;
	_matrix m_PreWroldMat = XMMatrixIdentity();

protected:
	unordered_map<wstring, class CComponent*>		m_Components;
protected:
	virtual HRESULT SetUp_Components(_uint iLevelIndex, const wstring& pPrototypeTag, const wstring& pComponentTag, CComponent** ppOut, void* pArg = nullptr);
	virtual HRESULT SetUp_Components(const wstring& pComponentTag, CComponent* pClone);
public:
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg) = 0;
	virtual void Free() override;
};

END