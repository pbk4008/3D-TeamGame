#pragma once
#ifndef __STATIC_OBJECT_H__
#define __STATIC_OBJECT_H__
#include "GameObject.h"
#include "Collider.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CHierarchyNode;
END
BEGIN(Tool)
class CToolObject final : public CGameObject
{
public:
	enum class TYPE {TREE,ROCK,GRASS, CHARACTER, MONSTER,TYPE_END};
	typedef struct tagCollider
	{
		CCollider* pCollider;
		_bool bBone;
	}COLLIDER;
	typedef struct tagSaveData
	{
		_uint eType;
		_float3 vScale;
		_float3 vPosition;
		_bool bBone;
		char szBone[MAX_PATH];
	}COLLIDERDATA;
private:
	explicit CToolObject();
	explicit CToolObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CToolObject(const CToolObject& rhs);
	virtual ~CToolObject() = default;
public:
	virtual HRESULT Init_GameObject(COMINDEX * pIndex);
	virtual HRESULT Init_Clone(void* pArg);
	virtual _int Update_GameObject(_float fDeltaTime);
	virtual HRESULT Render_GameObject();
public:
	void Add_Collider(CCollider* pCollider, _bool bBoneCheck);
	void AddBoneCollider(const char* pBoneName);
	void DeleteCollider(_uint iIndex);
	HRESULT Save_Collider(const _tchar* pFileName);
	HRESULT Load_Collider(const _tchar* pFileName);
private:
	_fmatrix Calcul_BonMatrix(CHierarchyNode* pBone);
public:
	static CToolObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, COMINDEX* pIndex);
	virtual CGameObject* Clone(void* pArg=nullptr);
public:
	void setType(TYPE eType) { m_eType = eType; }
	void setAnimationIndex(_uint iIndex) { m_pModel->SetUp_AnimationIndex(iIndex); }
	void setPos(_vector vPos);
	void setRotate(_vector vRotate);
	void setScale(_vector vScale);
	void Change_TransformValue(_vector& vScale, _vector& vRotate, _vector& vPos);
	void Change_ColliderValue(_uint iIndex ,_vector& vScale, _vector& vRotate, _vector& vPos);
	void setColliderValue(_uint iIndex, _fvector vScale, _fvector vRotate, _fvector vPos);
	void ResetColliderColor();
	void setMeshNum(_uint iIndex) { m_iMeshNum = iIndex; }
	void setObjName(const _tchar* pName) { lstrcpy(m_szObjName, pName); }
	void setMatrix(_fmatrix matTransform);
public:
	TYPE getType() { return m_eType; }
	CModel::TYPE getModelType() { return m_pModel->getModelType(); }
	vector<COLLIDER*>* getVecCollider() { return &m_vecCollider; }
	vector<class CAnimation*>* getAnimations() { return m_pModel->getAnimations(); }
	CCollider::COLLIDER_TYPE getColliderType(_uint iIndex) { return m_vecCollider[iIndex]->pCollider->getType(); }
	CCollider* getClickCollider() { return m_vecCollider[0]->pCollider; }
	CCollider* getClickCollider(_uint iIndex) { return m_vecCollider[iIndex]->pCollider; }
	_fmatrix getMatrix() { return m_pTransform->getWorldMatrix(); }
	_float3 getRotateValue() { return m_vRotateValue; }
	_uint getMeshNum() { return m_iMeshNum; }
	vector<CHierarchyNode*>* getBones() { return m_pModel->getBones(); }
	_tchar* getObjName() { return m_szObjName; }
private:
	virtual void Free();
private:
	CModel* m_pModel;
	TYPE m_eType;
	_uint m_iMeshNum;
	_float3 m_vRotateValue;
	vector<COLLIDER*> m_vecCollider;
	vector<CHierarchyNode*> m_vecBone;
	_bool m_bClicked;
	_tchar m_szObjName[MAX_PATH];
};
#endif
END