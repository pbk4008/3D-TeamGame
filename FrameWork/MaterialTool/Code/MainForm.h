#pragma once



// CMainForm 폼 보기
class CMaterialTool;
BEGIN(Tool)
class CModelObject;
END
BEGIN(Engine)
class CMouse;
class CMaterial;
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
	_int Update_Material();
	_int Update_AddTextureList();
	_uint Update_ShaderFile();
	_uint Update_Type();
	_uint Click_RadioBtn();
private:
	HRESULT Start_Level();
	CModelObject* Find_Model(const wstring& pModelName);
	_bool Check_LoadFile(const wstring& pFileName);
	HRESULT Focusing();
	HRESULT Picking();
	HRESULT Load_Material(CModelObject* pObj);
private:
	_int Check_RadioBtn();
private:
	HRESULT Ready_Tree();
	HRESULT Ready_ComboBox();
	HRESULT Ready_CommonTexture();
private:
	void Free();
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	CRenderer* m_pRenderer;
	CModelObject* m_pSelModel;
	CMaterial* m_pSelMaterial;
	_int m_iSelMeshIndex;
	_int m_iSelShaderFile;
	_int m_iCurRadioBtn;
	_int m_iPreRadioBtn;
private:
	CString m_tFbxPath;
	CString m_tFbxName;
	_int m_iSelMaterialIndex;
	_int m_iPreMaterialIndex;
	_bool m_bChange;
	_bool m_bTextureChange;
private:
	CTreeCtrl m_FbxTree;
	CListBox m_TextureBox;
	CListBox m_MaterialBox;
	CListBox m_AddTextureList;
	CComboBox m_ShaderFileComboBox;
private:
	CEdit m_tMaterialNameEdit;
	CEdit m_tTextureTypeEdit;
	CButton m_tStaticBtn;
	CButton m_tAnimBtn;
	CButton m_tInstance_StaticBtn;
	CButton m_tInstance_AnimBtn;
public:
	afx_msg void OnMaterialAddBtnClick();
	afx_msg void OnTextureAddBtnClick();
	afx_msg void OnMaterialApplyBtnClick();
	afx_msg void OnMeshSaveBtnClick();
	afx_msg void OnChangeShaderFileBtnClick();
};


