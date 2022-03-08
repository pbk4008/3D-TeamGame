#pragma once



// CMainForm 폼 보기
class CMaterialTool;
BEGIN(Tool)
class CModelObject;
END
BEGIN(Engine)
class CMouse;
END
class CMainForm : public CFormView
{
	DECLARE_DYNCREATE(CMainForm)
protected:
	CMainForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMainForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMainForm };
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
public:
	_int Tick(_double dDeltaTime);
	HRESULT Render();
private:
	_int Update_Tree();
	_int Update_TextureList();
private:
	HRESULT Start_Level();
	HRESULT Ready_Tree();
	CModelObject* Find_Model(const wstring& pModelName);
	HRESULT Focusing();
	HRESULT Picking();
private:
	void Free();
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	CRenderer* m_pRenderer;
	CMouse* m_pMouse;
private:
	CString m_tFbxPath;
	CString m_tFbxName;
	CModelObject* m_pSelModel;
	_bool m_bChange;
private:
	CTreeCtrl m_FbxTree;
public:
	CListBox m_TextureBox;
};


