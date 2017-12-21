#pragma once
#include "StageButton.h"

// CDlgPTZCtrl 对话框

class CDlgPTZCtrl : public CDialog
{
	DECLARE_DYNAMIC(CDlgPTZCtrl)

public:
	CDlgPTZCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPTZCtrl();

// 对话框数据
	enum { IDD = IDD_DLG_PTZCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	LPVOID m_pParent;

	CStageButton btn_ptzup;//上
	CStageButton btn_ptzleft;//左
	CStageButton btn_ptzright;//右
	CStageButton btn_ptzdown;//下

	CStageButton btn_ptzshort;//缩
	CStageButton btn_ptzlong;//伸
	CStageButton btn_ptznear;//近
	CStageButton btn_ptzfar;//远
	CStageButton btn_ptzsmall;//小
	CStageButton btn_ptzbig;//大
	CStageButton btn_ptzSetp;//设置预置点
	CStageButton btn_ptzGetp;//调用预置点
	CStageButton btn_ptzDelp;//删除预置点
	CStageButton btn_ptzDelAllp;//删除所有预置点
	CStageButton btn_ptzCruise;//巡航

	CSliderCtrl m_splSpeed;	//转速进度条
	CComboBox m_cmbPoint;	//预置点下拉框
	CComboBox m_cmbTrack;	//轨迹下拉框

	int		m_SelPoint;
	int		m_SelTrack;
	int		m_nSpeed;
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnStgBtnLButtonDown(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnStgBtnLButtonUp(WPARAM wParam,LPARAM lParam);
	virtual BOOL OnInitDialog();
};
