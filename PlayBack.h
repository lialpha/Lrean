#pragma once
#define CAPDATAPB			0			//从采集端调数据回放
#define SEVDATAPB			1			//从服务器调数据回放
// CVideoPlay 对话框

//当前的回放状态回调
enum StatusType
{
	PLAY_ST = WM_USER + 100,//开始状体提示（所跟参数1为开始，0为结束）
	PAUSE_ST,				//暂停状体提示（所跟参数1为暂停，0为开始）
	CHANGEAVI_ST,			//保存AVI状态提示（所跟参数1为开始，0为结束）
	CHANGESPEED_ST,			//改变播放速度（所跟参数1为常速，+2为2倍加速，-2为两倍减速）（2、4、8，-2、-4）

	SETSLDMAXVALUE_ST,		//设置进度条最大值（文件个数，一个文件最长录制一分钟,采集端调录像模式试用）
	SETSLIDERPOS_ST,		//设置进度条位置
	SETTIMES_ST,			//设置当前播放时间

	CAPTURE_ST,				//截图返回状态
	CLOSE_ST,				//关闭界面

};

// PlayBack 对话框

class PlayBack : public CDialog
{
	DECLARE_DYNAMIC(PlayBack)

public:
	PlayBack(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PlayBack();

// 对话框数据
	enum { IDD = IDD_DIALOG_PlayBack };

public:

	LPVOID  m_pDlg;
	HANDLE	m_hWorkThread;
	DWORD	m_dWordThreadID;
	LPVOID	m_pParentWnd;
	int		m_nPanelIndex;
	CStatic	m_ShowViewWnd;
	CString	m_sDir;							//存放AVI文件以及抓图的路径

	int		m_nType;						//回放模式
	BOOL	m_bPause;						//暂停与播放
	BOOL	m_bStart;						//开始与结束
	BOOL	m_bStartSaveAVI;				//AVI文件保存的开关

	CTime	m_nStartDay;					//开始日期
	CTime	m_nStartTime;					//开始时间
	CTime	m_nStopDay;						//结束日期
	CTime	m_nStopTime;					//结束时间
	DATE	m_dNewTime;						//拖动产生的新播放时间
	int		m_nPos;							//拖动产生的新位置
	DATE	m_dBeginTime;					//开始时间
	DATE	m_dEndTime;						//结束时间

	CTimeSpan m_tSpan;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString GetDirectoryPath();
public:
	afx_msg void OnBnClickedBtPlay();
	afx_msg void OnBnClickedBtStop();
	afx_msg void OnBnClickedBtFsbw();
	afx_msg void OnBnClickedBtFsfw();
	afx_msg void OnBnClickedBtCapture();
	afx_msg void OnBnClickedBtSaveavi();
	afx_msg void OnBnClickedBtSetpath();
	afx_msg void OnBnClickedBtClear();
	virtual BOOL OnInitDialog();
};
