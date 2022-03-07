#pragma once



// CForm 폼 보기
BEGIN(Engine)
class CLayer;
END

BEGIN(Tool)
class CToolCamera;
class CToolMouse;
class CToolObject;
END
class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)
public:
	enum class TOOLS {TOOL_OBJECT, TOOL_MAP, TOOL_NAVIGATION, TOOL_CAM, TOOL_UI,TOOL_END};
protected:
	CForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
public:
	virtual ~CForm();
public:
	_int Update_Form(_float fDeltaTime);
	HRESULT Render_Form();
	HRESULT Init_ProtoTypeLayer(CGameObject* pObj);
	HRESULT Init_CloneLayer(CToolObject* pObj);
	CLayer* getLayer(Tool::TOOLLAYERID eLayerID);
	CDialog* getTools(TOOLS eTool) { return m_pTools[(_uint)eTool]; }
private:
	_int SelectUpdate(_float fDeltaTime); 
	HRESULT SelectRender();
private:
	CTabCtrl m_tTab;
	CDialog* m_pTools[(_uint)TOOLS::TOOL_END];
	CGameInstance* m_pGameInstance;
	CToolCamera* m_pCamera;
	CToolMouse* m_pMouse;
	_uint m_iIndex;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	map<Tool::TOOLLAYERID, CLayer*> m_mapLayer;
	CRenderer* m_pRenderer;
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
};


