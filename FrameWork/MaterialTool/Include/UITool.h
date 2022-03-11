#pragma once


// CUITool 대화 상자
#include "GameObject.h"
#include "ToolUI.h"
BEGIN(Tool)
class CToolCamera;
class CToolMouse;
END
class CUITool : public CDialog
{
	DECLARE_DYNAMIC(CUITool)
public:
	typedef struct tagUIData
	{
		_float2 vScale;
		_float2 vPos;
		_tchar szTextureTag[MAX_PATH];
		_uint iUITag;
	}UIDATA;
public:
	CUITool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CUITool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CToolMouse* pMouse,CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUITool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUITool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	_int Update_UITool(_float fDeltaTime);
	HRESULT Render_UITool();
private:
	_bool NullEditCheck();
	void ClassyfyUITag(CToolUI::UITAG eTag, CString& pStr);
	HRESULT Ready_UITexture();
	HRESULT SetUITexture(CString pStr);
	void ResetIndex(CGameObject::COMINDEX* pIndex);
	void Release();
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	CToolCamera* m_pOthoGraphiCam;
	CToolMouse* m_pMouse;
	vector<CToolUI*> m_vecUI;
private:
	CEdit m_tScale[2];
	CEdit m_tPos[2];
	_float2 m_vScale;
	_float2 m_vPos;
private:
	CListBox m_tTextureList;
	CStatic m_tPictureCtrl;
	CComboBox m_tUITagComboBox;
	CListBox m_tUIList;
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnUIAddBtnClick();
	afx_msg void OnUIDeleteBtnClick();
	afx_msg void OnUISaveBtnClick();
	afx_msg void OnUILoadBtnClick();
};
