#pragma once


// CObjectTool 대화 상자
#include "ToolObject.h"
#include "Model.h"
#include "Collider.h"
BEGIN(Engine)
class CLayer;
END
BEGIN(Tool)
class CToolMouse;
END
class CObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CObjectTool)
	typedef struct tagObjDesc
	{
		CModel::MODELDESC tModelDesc;
		CCollider::COLLIDERDESC tColliderDesc;
	}OBJDESC;
public:
	CObjectTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CObjectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CToolMouse* pMouse,CWnd* pParent = nullptr);
	virtual ~CObjectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CObjectTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSphereBtnClick();
	afx_msg void OnBoxBtnClick();
public:
	_int Update_ObjectTool(_float fDeltaTime);
	HRESULT Render_ObjectTool();
private:
	void CheckCollisionBtn();
	_bool EditNullCheck();
	void Release();
	HRESULT Deform_Model(OBJDESC tDesc, _uint iType, CToolObject** pObj);
	HRESULT Init_Object();
	HRESULT Add_ObjTree();
	HRESULT Ready_Root();
	HRESULT Ready_Tree();
	HRESULT Ready_Rock();
	HRESULT Ready_Grass();
	HRESULT Add_Bone(CToolObject* pObj);
	HRESULT Update_ColliderList(_int iSelectIndex, CToolObject* pObj);
	HRESULT Update_ColliderValue(CToolObject* pObj);
	void Update_AnimationList(_int iIndex, CToolObject* pObj);
	void Change_ColliderValue(CToolObject* pObj);
	void Change_Animation(CToolObject* pObj);
	_int getObjSelectIndex(HTREEITEM tSelItem);
	HTREEITEM getNode(HTREEITEM RootItem, HTREEITEM SelItem);
	_bool Check_ObjNameWithItem();
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
private:
	CToolMouse* m_pMouse;
private:
	CButton m_tSphereBtn;
	CButton m_tBoxBtn;
	_bool m_bCollisionCheck;
	_int m_iColliderSelectIndex;
	_int m_iSelectObjectIndex;
	_int m_iSelectAnimIndex;
private:
	_float3 m_vSpherePos;
	_float m_fSphereRadius;
private:
	_float3 m_vBoxScale;
	_float3 m_vBoxRotate;
	_float3 m_vBoxPos;
private:
	CStatic m_tSphereGroup;
	CStatic m_tSpherePos[3];
	CStatic m_tSphereRadius;

	CEdit m_tSpherePosEdit[3];
	CEdit m_tSphereRadiusEdit;
private:
	CStatic m_tBoxGroups[3];

	CStatic m_tBoxScale[3];
	CStatic m_tBoxRotate[3];
	CStatic m_tBoxPos[3];

	CEdit m_tBoxScaleEdit[3];
	CEdit m_tBoxRotateEdit[3];
	CEdit m_tBoxPosEdit[3];

	CTreeCtrl m_ObjTree;
	HTREEITEM m_tSelectItem;
private:
	CListBox m_tCollisionList;
	CComboBox m_tAnimationList;
public:
	afx_msg void OnColliderInsertBtnClick();
	afx_msg void OnCollisionDeleteBtnClick();
	afx_msg void OnColliderSaveBtnClick();
	afx_msg void OnCollliderLoadBtnClick();
};
