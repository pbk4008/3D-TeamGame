#pragma once


// CActionCamTool 대화 상자
BEGIN(Tool)
class CToolMouse;
END
class CActionCamTool : public CDialog
{
	DECLARE_DYNAMIC(CActionCamTool)
public:
	CActionCamTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CActionCamTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CToolMouse* pMouse, CWnd* pParent = nullptr);
	virtual ~CActionCamTool();


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CActionCamTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	typedef struct tagCameraInfo
	{
		_float3 vPos;
		_float3 vTarget;
		tagCameraInfo()
		{
			ZeroMemory(&vPos, sizeof(_float3));
			ZeroMemory(&vTarget, sizeof(_float3));
		}
	}CAMERAINFO;
public:
	_int Update_ActionCamTool(_float fDeltaTime);
	HRESULT Render_ActionCamTool();
private:
	void Release();
	_bool NullEditCheck();
	HRESULT Init_CameraInfo(_int iIndex);
	HTREEITEM getParent(_int iIndex);
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	vector<CAMERAINFO*> m_vecCameraInfo;
private:
	CToolMouse* m_pMouse;
private:
	CTreeCtrl m_tCameraTree;
private:
	_float3 m_vPos;
	_float m_fSpeed;
private:
	CEdit m_tPos[3];
	CEdit m_tSpeed;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCameraCreateBtn();
	afx_msg void OnCameraDeleteBtn();
};
