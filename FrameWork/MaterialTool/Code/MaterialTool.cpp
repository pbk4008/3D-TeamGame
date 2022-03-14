// AnimationTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YASIC.h"
#include "MaterialTool.h"
#include "afxdialogex.h"


// CAnimationTool 대화 상자

IMPLEMENT_DYNAMIC(CMaterialTool, CDialogEx)

CMaterialTool::CMaterialTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MaterialTool, pParent)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
{

}

CMaterialTool::CMaterialTool(ID3D11Device* pDevcie, ID3D11DeviceContext* pDeviceContext, CWnd* pParent)
	: m_pDevice(pDevcie)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CMaterialTool::~CMaterialTool()
{
	Free();
}

_uint CMaterialTool::Tick(_float fDeltatTime)
{
	return _uint();
}

void CMaterialTool::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

void CMaterialTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMaterialTool, CDialogEx)
END_MESSAGE_MAP()


// CAnimationTool 메시지 처리기
