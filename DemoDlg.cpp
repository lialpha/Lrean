
// DemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo.h"
#include "DemoDlg.h"
#include "playback/PlayBack.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDemoDlg 对话框


CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dlgCameraList = NULL;
	m_dlgPTZCtrl = NULL;
	m_lLoginID = 0;
	m_nSelViewID = IDC_STATIC_VIEW1;
	
	::InitializeCriticalSection(&m_cs);
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_TAB_MODULE,m_tabModule);
 	DDX_Control(pDX,IDC_STATIC_VIEW1,m_cPlayView1);
 	DDX_Control(pDX,IDC_STATIC_VIEW2,m_cPlayView2);
 	DDX_Control(pDX,IDC_STATIC_VIEW3,m_cPlayView3);
 	DDX_Control(pDX,IDC_STATIC_VIEW4,m_cPlayView4);
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MODULE, &CDemoDlg::OnTcnSelchangeTabModule)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_MODULE, &CDemoDlg::OnTcnSelchangingTabModule)
	ON_MESSAGE(WM_VIEW_LBUTTONUP,&CDemoDlg::OnViewBtnLButtonUp)
	ON_MESSAGE(WM_VIEW_LBUTTONDOWN,&CDemoDlg::OnViewBtnLButtonDown)
	ON_MESSAGE(WM_VIEW_CLOSEVIEW,&CDemoDlg::OnViewCloseView)
	ON_MESSAGE(MSG_STATUS,&CDemoDlg::OnStatus)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CDemoDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_LOGOUT, &CDemoDlg::OnBnClickedBtnLogout)
	ON_BN_CLICKED(IDC_BTN_TOSMALL, &CDemoDlg::OnBnClickedBtnTosmall)
	ON_BN_CLICKED(IDC_BTN_TOBIG, &CDemoDlg::OnBnClickedBtnTobig)
	ON_BN_CLICKED(IDC_BTN_PlayBack, &CDemoDlg::OnBnClickedBtnPlayback)
END_MESSAGE_MAP()


// CDemoDlg 消息处理程序

BOOL CDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	_CrtSetBreakAlloc(2991);//调试内存泄露位置
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码	
	BYTE IP1 = 220,IP2 = 181,IP3 = 120,IP4 = 243;
	((CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS_SERVERIP)))->SetAddress(IP1,IP2,IP3,IP4);
	((CEdit *)(GetDlgItem(IDC_EDIT_SERVERPORT)))->SetWindowText(_T("8006"));
	((CEdit *)(GetDlgItem(IDC_EDIT_USER)))->SetWindowText(_T("demo"));
	((CEdit *)(GetDlgItem(IDC_EDIT_PASSWORD)))->SetWindowText(_T("demo"));

	m_tabModule.InsertItem(0,"监控点列表");
	m_dlgCameraList = new CDlgCameraList;
	m_dlgCameraList->m_pParent = this;
	m_dlgCameraList->Create(IDD_DLG_CAMERALIST,&m_tabModule);
	m_mapTabPageDlg[0] = m_dlgCameraList;

	m_tabModule.InsertItem(1,"云台控制");
	m_dlgPTZCtrl = new CDlgPTZCtrl;
	m_dlgPTZCtrl->m_pParent = this;
	m_dlgPTZCtrl->Create(IDD_DLG_PTZCTRL,&m_tabModule);
	m_mapTabPageDlg[1] = m_dlgPTZCtrl;

	CRect rect;
	m_tabModule.GetClientRect(&rect);
	m_tabModule.AdjustRect(FALSE,&rect);
	m_dlgCameraList->MoveWindow(&rect);
	m_dlgPTZCtrl->MoveWindow(&rect);

	m_mapTabPageDlg[0]->ShowWindow(SW_SHOW);
	m_tabModule.SetCurSel(0);
	
	m_cPlayView1.SetViewID(IDC_STATIC_VIEW1);
	m_cPlayView2.SetViewID(IDC_STATIC_VIEW2);
	m_cPlayView3.SetViewID(IDC_STATIC_VIEW3);
	m_cPlayView4.SetViewID(IDC_STATIC_VIEW4);
	m_mapViewWnd[IDC_STATIC_VIEW1] = &m_cPlayView1;
	m_mapViewWnd[IDC_STATIC_VIEW2] = &m_cPlayView2;
	m_mapViewWnd[IDC_STATIC_VIEW3] = &m_cPlayView3;
	m_mapViewWnd[IDC_STATIC_VIEW4] = &m_cPlayView4;

	m_nSelViewID = IDC_STATIC_VIEW1;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDemoDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	SZY_VIEW_DisConnect(m_lLoginID);
	if (m_dlgCameraList != NULL)
	{
		delete m_dlgCameraList;
		m_dlgCameraList = NULL;
	}
	if (m_dlgPTZCtrl != NULL)
	{
		delete m_dlgPTZCtrl;
		m_dlgPTZCtrl = NULL;
	}
	
	CDialog::OnCancel();
}

void CDemoDlg::OnTcnSelchangeTabModule(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_tabModule.GetCurSel();
	m_mapTabPageDlg[nIndex]->ShowWindow(SW_SHOW);

	*pResult = 0;
}

void CDemoDlg::OnTcnSelchangingTabModule(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_tabModule.GetCurSel();
	m_mapTabPageDlg[nIndex]->ShowWindow(SW_HIDE);
	*pResult = 0;
}

//SDK内的状态回调
void __stdcall MyProcStatus(void *caller,long lRealPlayID,int nType,long nParam)
{
	CDemoDlg * p = (CDemoDlg *)caller;	
	int nViewID = 0;
	if (!p->GetViewCtrlID(lRealPlayID,nViewID))
		return;
	
	StatusData *pStatusData = new StatusData();
	pStatusData->lRealPlayID = lRealPlayID;
	pStatusData->nParam = nParam;
	pStatusData->nType = nType;

	p->ProcStatus(pStatusData,nViewID);
	p->PostMessage(MSG_STATUS,WPARAM(pStatusData),LPARAM(nViewID));
	
	
	return ;
}

//登录
void CDemoDlg::OnBnClickedBtnLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sIP,sPort,sUser,sPassWord;
	BYTE IP1,IP2,IP3,IP4;
	((CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS_SERVERIP)))->GetAddress(IP1,IP2,IP3,IP4);
	sIP.Format("%d.%d.%d.%d",IP1,IP2,IP3,IP4);
	((CEdit *)(GetDlgItem(IDC_EDIT_SERVERPORT)))->GetWindowText(sPort);
	((CEdit *)(GetDlgItem(IDC_EDIT_USER)))->GetWindowText(sUser);
	((CEdit *)(GetDlgItem(IDC_EDIT_PASSWORD)))->GetWindowText(sPassWord);
	if (sIP.IsEmpty() || sPort.IsEmpty() || sUser.IsEmpty() || sPassWord.IsEmpty())
	{
		AfxMessageBox("ip、port、账号、密码等任意一项不能为空");
		return;
	}
	if(!SZY_VIEW_Init(MyProcStatus,this))
	{
		AfxMessageBox("SDK初始化失败，请重新登录");
		return;
	}
	char *ServerIP[2] = {{sIP.GetBuffer()},{"master.4006043110.com"}};
	m_lLoginID = SZY_VIEW_Connect(ServerIP,2,atoi(sPort),sUser.GetBuffer());
	if (m_lLoginID == 0)
	{
		AfxMessageBox("连接失败，请重新登录");
		return;
	}
	SDK_LoginBackData sLoginBackData;
	if(!SZY_VIEW_Login(m_lLoginID,sUser.GetBuffer(),sPassWord.GetBuffer(),sLoginBackData))
	{
		AfxMessageBox("登录失败，请重新登录");
		SZY_VIEW_DisConnect(m_lLoginID);
		return;
	}
	/*if (SZY_VIEW_CreateAudioObject(m_lLoginID))
	{
	}*/
	m_sLoginBackData = sLoginBackData;
	m_dlgCameraList->Init();
}

//鼠标弹起
LRESULT CDemoDlg::OnViewBtnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	return 0L;
}
//鼠标按下
LRESULT CDemoDlg::OnViewBtnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	m_nSelViewID = int(wParam);
	return 0L;
}

BOOL CDemoDlg::GetViewCtrlID(long nViewPlayID,int &nViewID)
{
	BOOL bResult = FALSE;
	::EnterCriticalSection(&m_cs);
	map<int,long>::iterator it = m_mapRealPlayView.begin();
	for (it; it != m_mapRealPlayView.end(); it ++)
	{
		if (it->second == nViewPlayID)
		{
			bResult = TRUE;
			nViewID = it->first;
			break;
		}
	}
	::LeaveCriticalSection(&m_cs);
	return bResult;
}

void CDemoDlg::CloseView(int nViewID)
{
	::EnterCriticalSection(&m_cs);
	if (m_mapRealPlayView.find(nViewID) != m_mapRealPlayView.end() && m_mapRealPlayView[nViewID] != 0)
	{
		SZY_VIEW_StopRealPlay(m_lLoginID,m_mapRealPlayView[nViewID]);
		m_mapRealPlayView[nViewID] = 0;
	}
	::LeaveCriticalSection(&m_cs);
	CloseAudio();
}

void CDemoDlg::ClearView(int nViewID)
{
	::EnterCriticalSection(&m_cs);
	if (m_mapRealPlayView.find(nViewID) != m_mapRealPlayView.end() && m_mapRealPlayView[nViewID] != 0)
		m_mapRealPlayView[nViewID] = 0;
	::LeaveCriticalSection(&m_cs);
}

void CDemoDlg::OpenView(long nViewPlayID,int nViewID)
{
	::EnterCriticalSection(&m_cs);
	m_mapRealPlayView[nViewID] = nViewPlayID;
	::LeaveCriticalSection(&m_cs);
}

void CDemoDlg::CloseAudio()
{
	::EnterCriticalSection(&m_cs);
	map<int,long>::iterator it = m_mapRealPlayView.begin();
	for (it; it != m_mapRealPlayView.end(); it ++)
	{
		SZY_VIEW_CloseAudioListen(m_lLoginID,it->second);
	}
	::LeaveCriticalSection(&m_cs);
}

LRESULT CDemoDlg::OnViewCloseView(WPARAM wParam,LPARAM lParam)
{
	CloseView(int(wParam));
	return 0L;
}

void CDemoDlg::ProcStatus(StatusData *pStatusData,int nViewID)
{
	switch (pStatusData->nType)//状态的逻辑处理
	{
	case VIEW_START://开启视频操作
		if (pStatusData->nParam == 0)
			;
		else if(pStatusData->nParam == -1)
			;
		else if(pStatusData->nParam == -2)
			;
		else if(pStatusData->nParam == 1)
			;
		break;
	case VIEW_STOP://关闭视频操作
		if (pStatusData->nParam == 0)
			ClearView(nViewID);
		break;
	}
}

void CDemoDlg::UpdateNowCamera(TreeSXT sCameraInfo)
{
	m_sNowCameraInfo = sCameraInfo;
}

LRESULT CDemoDlg::OnStatus(WPARAM wParam,LPARAM lParam)
{
	StatusData *pStatusData = (StatusData *)wParam;
	int nViewID = (int) lParam;
	CStatic *pShowView = (CStatic *)(GetDlgItem(nViewID));
	switch (pStatusData->nType)//状态的界面处理
	{
	case VIEW_START://开启视频操作
		if (pStatusData->nParam == 0)
			pShowView->SetWindowText(_T("等待中..."));//窗口显示视频开启过程状态
		else if(pStatusData->nParam == -1)
			pShowView->SetWindowText(_T("对象参数初始化失败"));//窗口显示视频开启过程状态
		else if(pStatusData->nParam == -2)
			pShowView->SetWindowText(_T("观看人数超过最大值"));
		else if(pStatusData->nParam == 1)
			pShowView->SetWindowText(_T("开始"));
		break;
	case VIEW_STOP://关闭视频操作
		if (pStatusData->nParam == 0)
		{
			pShowView->SetWindowText(_T("停止"));//窗口显示视频关闭状态
		}
		break;
	}
	delete pStatusData;
	pStatusData = NULL;
	return 0L;
}

void CDemoDlg::OnBnClickedBtnLogout()
{
	// TODO: 在此添加控件通知处理程序代码
	SZY_VIEW_DisConnect(m_lLoginID);
}

void CDemoDlg::OnBnClickedBtnTosmall()
{
	// TODO: 在此添加控件通知处理程序代码
	::EnterCriticalSection(&m_cs);
	CRect rect;
	map<int,CPlayView*>::iterator it = m_mapViewWnd.begin();
	if (it != m_mapViewWnd.end())
	{
		((CPlayView *)GetDlgItem(it->second->m_nViewID))->GetWindowRect(&rect);
		this->ScreenToClient(&rect);
		rect.bottom -= 10;
		rect.right -= 10;
		it->second->MoveRect(rect);
	}
	::LeaveCriticalSection(&m_cs);
}

void CDemoDlg::OnBnClickedBtnTobig()
{
	// TODO: 在此添加控件通知处理程序代码
	::EnterCriticalSection(&m_cs);
	CRect rect;
	map<int,CPlayView*>::iterator it = m_mapViewWnd.begin();
	if (it != m_mapViewWnd.end())
	{
		((CPlayView *)GetDlgItem(it->second->m_nViewID))->GetWindowRect(&rect);
		this->ScreenToClient(&rect);
		rect.bottom += 10;
		rect.right += 10;
		it->second->MoveRect(rect);
	}
	::LeaveCriticalSection(&m_cs);
}

void CDemoDlg::OnBnClickedBtnPlayback()
{
	// TODO: 在此添加控件通知处理程序代码
	if (strlen(m_sNowCameraInfo.SXT_id) == 0)
	{
		AfxMessageBox("请选择要回放的摄像头");
		return;
	}

	PlayBack DlgPlayBack;
	
	DlgPlayBack.SetLoginID(m_lLoginID);
	DlgPlayBack.SetPBCameraInfo(m_sNowCameraInfo.JieDian_id,m_sNowCameraInfo.SXT_id,m_sNowCameraInfo.Server_ip,m_sNowCameraInfo.Server_Port);
	DlgPlayBack.DoModal();
}
