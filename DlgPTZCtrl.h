#pragma once
#include "StageButton.h"

// CDlgPTZCtrl �Ի���

class CDlgPTZCtrl : public CDialog
{
	DECLARE_DYNAMIC(CDlgPTZCtrl)

public:
	CDlgPTZCtrl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPTZCtrl();

// �Ի�������
	enum { IDD = IDD_DLG_PTZCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	LPVOID m_pParent;

	CStageButton btn_ptzup;//��
	CStageButton btn_ptzleft;//��
	CStageButton btn_ptzright;//��
	CStageButton btn_ptzdown;//��

	CStageButton btn_ptzshort;//��
	CStageButton btn_ptzlong;//��
	CStageButton btn_ptznear;//��
	CStageButton btn_ptzfar;//Զ
	CStageButton btn_ptzsmall;//С
	CStageButton btn_ptzbig;//��
	CStageButton btn_ptzSetp;//����Ԥ�õ�
	CStageButton btn_ptzGetp;//����Ԥ�õ�
	CStageButton btn_ptzDelp;//ɾ��Ԥ�õ�
	CStageButton btn_ptzDelAllp;//ɾ������Ԥ�õ�
	CStageButton btn_ptzCruise;//Ѳ��

	CSliderCtrl m_splSpeed;	//ת�ٽ�����
	CComboBox m_cmbPoint;	//Ԥ�õ�������
	CComboBox m_cmbTrack;	//�켣������

	int		m_SelPoint;
	int		m_SelTrack;
	int		m_nSpeed;
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnStgBtnLButtonDown(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnStgBtnLButtonUp(WPARAM wParam,LPARAM lParam);
	virtual BOOL OnInitDialog();
};
