
// DemoDlg.h : 头文件
//

#pragma once
#include "DlgPTZCtrl.h"
#include "DlgCameraList.h"
#include "PlayView.h"

#include <map>
using namespace std;

#define MSG_STATUS WM_USER + 100
struct StatusData
{
	long lRealPlayID;
	int nType;
	long nParam;
	StatusData()
	{
		lRealPlayID = 0;
		nType = 0;
		nParam = 0;
	}
};

// CDemoDlg 对话框
class CDemoDlg : public CDialog
{
// 构造
public:
	CDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl		m_tabModule;
	CDlgPTZCtrl		*m_dlgPTZCtrl;
	CDlgCameraList	*m_dlgCameraList;
	map<int,CDialog *> m_mapTabPageDlg;
	
	SDK_LoginBackData	m_sLoginBackData;
	long			m_lLoginID;
	int				m_nSelViewID;
	map<int,long>	m_mapRealPlayView;//播放类对象与播放窗口的关联
	CRITICAL_SECTION m_cs;

	CPlayView		m_cPlayView1;
	CPlayView		m_cPlayView2;
	CPlayView		m_cPlayView3;
	CPlayView		m_cPlayView4;

	map<int,CPlayView*> m_mapViewWnd;

	TreeSXT m_sNowCameraInfo;

	
protected:
	virtual void OnCancel();
public:
	BOOL GetViewCtrlID(long nViewPlayID,int &nViewID);
	void CloseView(int nViewID);
	void ClearView(int nViewID);
	void OpenView(long nViewPlayID,int nViewID);
	void CloseAudio();
	afx_msg LRESULT OnStatus(WPARAM wParam,LPARAM lParam);
	void ProcStatus(StatusData *pStatusData,int nViewID);
	void UpdateNowCamera(TreeSXT sCameraInfo);

	afx_msg void OnTcnSelchangeTabModule(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingTabModule(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnLogin();
	afx_msg LRESULT OnViewBtnLButtonDown(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnViewBtnLButtonUp(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnViewCloseView(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedBtnLogout();
	afx_msg void OnBnClickedBtnTosmall();
	afx_msg void OnBnClickedBtnTobig();
	afx_msg void OnBnClickedBtnPlayback();
};
