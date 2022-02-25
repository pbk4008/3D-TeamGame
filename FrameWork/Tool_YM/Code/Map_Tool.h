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
	void InitAssetsTree(void);

public:
	FILEINFO  m_FileInfo;
	_bool	  m_bSelect_FBXFile = false;

public:
	CTreeCtrl m_AssetTree;

public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
};
