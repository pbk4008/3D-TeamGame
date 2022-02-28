#pragma once

#include "Menu_Form.h"

// CMap_Tool 대화 상자

class CMap_Tool : public CDialogEx
{
	DECLARE_DYNAMIC(CMap_Tool)

public:
	CMap_Tool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMap_Tool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMap_Tool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

//개발자 재정의

public:
	void			 InitAssetsTree(void);
	void			 InitHierarchyTree(void);
	void			 Clear_Tree(void);
	HTREEITEM		 Find_TreeData(HTREEITEM& hItem, CString findItem);
	HRESULT			 Delete_Item(_uint _iLevelIndex, const MODELDESC& _ModelDesc);
	HRESULT			 Create_HierarchyTree(const MODELDESC& _ModelInfo);
	HRESULT			 Delete_HierarchyTreeItem(const MODELDESC& _ModelDesc);
	_int			 Find_TreeItem_Indx(HTREEITEM hSelectItem);
	_bool			 Get_HasChild(CTreeCtrl& tTree, HTREEITEM tParent, HTREEITEM tSelect);
	static HTREEITEM Search_ParentItemInTree(CTreeCtrl* _TreeCtrl, CString _FindStr, HTREEITEM _pParent);

public:
	FILEINFO		  m_FileInfo;
	MODELDESC		  m_ModelDesc;
	_bool			  m_bSelect_FBXFile = false;

public:
	/* 원본 모델 중복생성 방지 태그 */
	list<wstring>	    m_ProtoTag;
	/* Hierarchy Tree 생성용 태그 */
	vector<wstring>			  m_vTag;
	vector<wstring>			  m_vModelName;

public:
	CTreeCtrl		  m_AssetTree;
	CTreeCtrl		  m_HierarchyTree;
	HTREEITEM		  m_TreeRoot;

public:
	class CMainFrame* m_pMain = nullptr;
	class CMenu_Form* m_pMenuForm = nullptr;
	
public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnNMRRClickHierTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void Delete_Clone_Model();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
