#pragma once


// CAnimationTool 대화 상자

class CMaterialTool : public CDialogEx
{
	DECLARE_DYNAMIC(CMaterialTool)

public:
	CMaterialTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CMaterialTool(ID3D11Device*pDevcie, ID3D11DeviceContext* pDeviceContext, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMaterialTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum {	IDD = IDD_MaterialTool};
#endif
public:
	_uint Tick(_float fDeltatTime);
private:
	void Free();
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
};
