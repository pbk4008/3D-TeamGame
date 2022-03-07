#pragma once


// CNavigationTool 대화 상자
BEGIN(Tool)
class CToolMouse;
class CToolTerrain;
class CToolCell;
class CToolPoint;
END
class CNavigationTool : public CDialog
{
	DECLARE_DYNAMIC(CNavigationTool)
public:
	typedef struct tagCellData
	{
		_float3 p1Position;
		_float3 p2Position;
		_float3 p3Position;
	}CELLDATA;
public:
	CNavigationTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CNavigationTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CToolMouse* pMouse, CWnd* pParent = nullptr);
	virtual ~CNavigationTool();



// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CNavigationTool };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	_int Update_NavigationTool(_float fDeltaTime);
	HRESULT Render_NavigationTool();
private:
	HRESULT Ready_CellTree();
	_bool NullEditCheck();
	void CheckMode();
	HRESULT Click_Mouse();
	HRESULT Create_Point(_fvector vPos);
	HRESULT Create_Cell();
	_bool Check_SamePointClick();
	void Update_PositionValue();
	void CellTreeClick(_uint iIndex);
private:
	void Release();
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
private:
	CToolMouse* m_pMouse;
	CToolTerrain* m_pTerrain;
	_bool m_bClick;
	_int m_iSelectCell;
	_int m_iMouseSelect;
	_int m_iDragCell;
	_bool m_bPointClick;
private:
	_float3 m_vPosition;
	_bool m_bModeCheck;
	CEdit m_tPosition[3];
	
	CTreeCtrl m_tNaviMeshTree;
	CButton m_tCreateMode;
	CButton m_tModifyMode;
	vector<CToolPoint*> m_vecTmpPoints;
	vector<CToolCell*> m_vecToolCell;
	vector<CToolObject*>* m_pStaticObjects;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCellDeleteBtnClick();
	afx_msg void OnNavimeshSaveBtnClick();
	afx_msg void OnNaviMeshLoadBtnClick();
};
