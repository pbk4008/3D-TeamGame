﻿
// UITool.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "UITool.h"
#include "MainFrm.h"

#include "UIToolDoc.h"
#include "UIToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUIToolApp

BEGIN_MESSAGE_MAP(CUIToolApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CUIToolApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CUIToolApp 생성

CUIToolApp::CUIToolApp() noexcept
{

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 애플리케이션을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("UITool.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CUIToolApp 개체입니다.

CUIToolApp theApp;


// CUIToolApp 초기화

BOOL CUIToolApp::InitInstance()
{
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	// 애플리케이션의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CUIToolDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CUIToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CUIToolView* pView = dynamic_cast<CUIToolView*>(pMain->m_SplitterWnd.GetPane(0, 1));

	g_hWnd = pView->m_hWnd;
	g_hInst = AfxGetInstanceHandle();

#ifdef _DEBUG
	AllocConsole();
	FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;

	freopen_s(&fpstdin, "CONIN$", "r", stdin);
	freopen_s(&fpstdout, "CONOUT$", "w", stdout);
	freopen_s(&fpstderr, "CONOUT$", "w", stderr);
#endif



	m_pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_pGameInstance->Initialize_Engine(g_hInst, g_hWnd, 1, CGraphic_Device::WINMODE::MODE_WIN, WINCX, WINCY, &m_pDevice, &m_pDeviceContext))
	{
		ERR_MSG(L"Failed to Initialize Engine In CUIToolApp::InitInstance() ");
		return false;
	}

	if (FAILED(m_pGameInstance->Ready_Timer(TEXT("Timer_Dafault"))))
	{
		ERR_MSG(L"Failed to Ready Default Timer");
		return false;
	}
	if (FAILED(m_pGameInstance->Ready_Timer(TEXT("Timer_60Frame"))))
	{
		ERR_MSG(L"Failed to Ready 60Frame Timer");
		return false;
	}

	return TRUE;
}

int CUIToolApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.

#ifdef _DEBUG
	FreeConsole();
#endif

	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

int CUIToolApp::Engine_Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
	{
		return -1;
	}

	m_pGameInstance->Tick_Engine(TimeDelta);

	return 0;
}

HRESULT CUIToolApp::Engine_Render()
{
	if (nullptr == m_pGameInstance)
	{
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Clear_BackBuffer_View(XMFLOAT4(0.f, 0.5f, 0.5f, 1.f))))
	{
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Clear_DepthStencil_View()))
	{
		return E_FAIL;
	}

	///* 내 게임을 구성하는 객체들의 렌더 함수를 호출한다 */
	//if (FAILED(m_pRenderer->Draw_RenderGroup()))
	//{
	//	return E_FAIL;
	//}

	/* 내 게임내의 기타 등등을 렌더링한다 */
	if (FAILED(m_pGameInstance->Render_Engine()))
	{
		return E_FAIL;
	}


	if (FAILED(m_pGameInstance->Present()))
	{
		return E_FAIL;
	}

	return S_OK;
}

// CUIToolApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CUIToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



BOOL CUIToolApp::OnIdle(LONG lCount)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_pMainWnd->IsIconic())
	{
		return false;
	}

	m_pGameInstance->Update_TimeDelta(TEXT("Timer_Dafault"));
	m_TimerAcc += (_float)m_pGameInstance->Get_TimeDelta(TEXT("Timer_Dafault"));

	if (m_TimerAcc >= 1.0 / 60.f)
	{
		m_TimerAcc = 0.0;

		m_pGameInstance->Update_TimeDelta(TEXT("Timer_60Frame"));

		if (0 > Engine_Tick(m_pGameInstance->Get_TimeDelta(TEXT("Timer_60Frame"))))
		{
			ERR_MSG(L"Failed CUIToolApp::OnIdle Tick");
			return false;
		}

		if (FAILED(Engine_Render()))
		{
			ERR_MSG(L"Failed  CUIToolApp::OnIdle Render");
			return false;
		}
	}

	return true;
}
