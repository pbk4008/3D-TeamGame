#pragma once


// CMapTool 대화 상자
#include "ToolObject.h"
BEGIN(Engine)
class CSaveManager;
class CLayer;
END
BEGIN(Tool)
class CToolMouse;
class CToolTerrain;
END
class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)
public:
	enum class DRAWCHECK {DRAW_HEIGHT, NO_DRAW,DRAW_END};
	enum class OBJCHECK {OBJ_STATIC, OBJ_DYNAMIC, OBJ_END};
public:
	typedef struct tagStaticSaveData
	{
		_uint iMeshNum;
		_float3 vRight;
		_float3 vUp;
		_float3 vLook;
		_float3 vPos;
	}STATICDATA;
	typedef struct tagDynamicSaveData
	{
		_uint iMeshNum;
		_float3 vPos;
	}DYNAMICDATA;
public:
	CMapTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CToolMouse* pMouse,CWnd* pParent = nullptr);
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMapTool };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	_int Update_MapTool(_float fDeltaTime);
	HRESULT Render_MapTool();
	CToolTerrain* getTerrain() {		return m_pTerrain; 	}
	vector<CToolObject*>* getStaticObjectVector() { return &m_vecStaticAddObjects; }
private:
	HRESULT Ready_MapTool();
	HRESULT ClassifyObjectList();
	void Release();
	_bool NullEditCheck();
	HRESULT Ready_StaticTree(CLayer* pLayer);
	HRESULT Ready_DynamicTree(CLayer* pLayer);
	HRESULT Set_TerrainTexture(CString pStr);
	HRESULT Ready_TerrainsTexture();
	HRESULT Insert_CloneObject(CToolObject* pObj,CString strName,_int iMeshNum);
	CToolObject* Load_CloneObject(CToolObject* pObj);
	void Update_Mouse();
	void Update_Transform();
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	vector<CToolObject*> m_vecStaticAddObjects;
	vector<CToolObject*> m_vecDynamicAddObjects;
private:
	CToolMouse* m_pMouse;
	CToolTerrain* m_pTerrain;
private:
	_int m_iSelectStaticIndex;
	_int m_iSelectDynamicIndex;
	_int m_iMouseSelectIndex;
	_float3 m_tScale;
	_float3 m_tRotate;
	_float3 m_tPosition;
	_float m_tMouseRadius;
	DRAWCHECK m_eTerrainDrawCheck;
	OBJCHECK m_eObjCheck;
	_bool m_bSameHeight;

	CEdit m_tScaleEdit[3];
	CEdit m_tRotateEdit[3];
	CEdit m_tPostionEdit[3];
	CEdit m_tMouseRadiusEdit;

	CListBox m_tTerrainTextureList;
	CStatic m_tTexture;

	CTreeCtrl m_tStaticObject;
	CTreeCtrl m_tDynamicObejct;
	CListBox m_tStaticObjAddList;
	CListBox m_tDynamicObjAddList;

	CButton m_tStaticCheck;
	CButton m_tDynamicCheck;
	CButton m_tSameHeight;
	CButton m_tTerrainDrawCheck[2];
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnAddBtnClick();
	afx_msg void OnDeleteBtnClick();
	afx_msg void OnHeightMapSaveBtnClick();
	afx_msg void OnHeightMapLoadBtnClick();
	afx_msg void OnStaticObjectSaveBtnClick();
	afx_msg void OnStaticObjectLoadBtnClick();
	afx_msg void OnDynamicSaveBtnClick();
	afx_msg void OnDynamicLoadBtnClick();
};
