// DlgPTZCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo.h"
#include "DlgPTZCtrl.h"
#include "DemoDlg.h"


// CDlgPTZCtrl �Ի���

IMPLEMENT_DYNAMIC(CDlgPTZCtrl, CDialog)

CDlgPTZCtrl::CDlgPTZCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPTZCtrl::IDD, pParent)
{
	m_pParent = NULL;
}

CDlgPTZCtrl::~CDlgPTZCtrl()
{
}

void CDlgPTZCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_SLIDER_SPEED,m_splSpeed);
	DDX_Control(pDX,IDC_COMBO_POINT,m_cmbPoint);
	DDX_Control(pDX,IDC_COMBO_TRACK,m_cmbTrack);

	DDX_Control(pDX,IDC_BTN_UP,btn_ptzup);
	DDX_Control(pDX,IDC_BTN_LEFT,btn_ptzleft);
	DDX_Control(pDX,IDC_BTN_RIGHT,btn_ptzright);
	DDX_Control(pDX,IDC_BTN_DOWN,btn_ptzdown);
	DDX_Control(pDX,IDC_BTN_SHORT,btn_ptzshort);
	DDX_Control(pDX,IDC_BTN_LONG,btn_ptzlong);
	DDX_Control(pDX,IDC_BTN_NEAR,btn_ptznear);
	DDX_Control(pDX,IDC_BTN_FAR,btn_ptzfar);
	DDX_Control(pDX,IDC_BTN_SMALL,btn_ptzsmall);
	DDX_Control(pDX,IDC_BTN_BIG,btn_ptzbig);
	DDX_Control(pDX,IDC_BTN_SETPOINT,btn_ptzSetp);
	DDX_Control(pDX,IDC_BTN_GOTOPOINT,btn_ptzGetp);
	DDX_Control(pDX,IDC_BTN_DELPOINT,btn_ptzDelp);
	DDX_Control(pDX,IDC_BTN_DELALLPOINT,btn_ptzDelAllp);
	DDX_Control(pDX,IDC_BTN_CRUISE,btn_ptzCruise);
}


BEGIN_MESSAGE_MAP(CDlgPTZCtrl, CDialog)
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_STGBTN_LBUTTONUP,&CDlgPTZCtrl::OnStgBtnLButtonUp)
	ON_MESSAGE(WM_STGBTN_LBUTTONDOWN,&CDlgPTZCtrl::OnStgBtnLButtonDown)
END_MESSAGE_MAP()


// CDlgPTZCtrl ��Ϣ�������


BOOL CDlgPTZCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	btn_ptzup.m_Type = ctUp;
	btn_ptzdown.m_Type = ctDown;
	btn_ptzleft.m_Type = ctLeft;
	btn_ptzright.m_Type = ctRight;
	btn_ptzshort.m_Type = ctReFoci;
	btn_ptzlong.m_Type = ctInFoci;
	btn_ptznear.m_Type = ctReLen;
	btn_ptzfar.m_Type = ctInLen;
	btn_ptzsmall.m_Type = ctReAperture;
	btn_ptzbig.m_Type = ctInAperture;

	btn_ptzSetp.m_Type = ctSetp;
	btn_ptzGetp.m_Type = ctGetp;
	btn_ptzDelp.m_Type = ctDelp;
	btn_ptzDelAllp.m_Type = ctDelAllp;
	btn_ptzCruise.m_Type = ctCruise;

	m_splSpeed.SetRange(0x00, 0x3F);
	m_splSpeed.SetLineSize(1);
	m_splSpeed.SetPageSize(4);
	m_splSpeed.SetPos(0x21);
	m_nSpeed = 0x21;
	CString sSpeed;
	sSpeed.Format("%d",m_nSpeed);
	((CStatic *)(GetDlgItem(IDC_STATIC_SPEED)))->SetWindowText(sSpeed);
	CString tmp;
	for (int i = 1; i < 16*16+1; i ++)
	{
		tmp.Format("%d",i);
		m_cmbPoint.AddString(tmp);
	}
	m_SelPoint = 1;
	m_cmbPoint.SetCurSel(0);
	for (int i = 0; i < 16; i ++)
	{
		tmp.Format("�켣%d(%d-%d)",i+1,i*16+1,i*16+16);
		m_cmbTrack.AddString(tmp);
	}
	m_SelTrack = 0;
	m_cmbTrack.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
//�϶�ת�ٽ�����
void CDlgPTZCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CString sSpeed;
	m_nSpeed = m_splSpeed.GetPos();
	sSpeed.Format("%d",m_nSpeed);
	((CStatic *)(GetDlgItem(IDC_STATIC_SPEED)))->SetWindowText(sSpeed);


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

}
//��굯��
LRESULT CDlgPTZCtrl::OnStgBtnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	CDemoDlg *pDlg = (CDemoDlg *)m_pParent;
	long lRealPlayID = pDlg->m_mapRealPlayView[pDlg->m_nSelViewID];
	SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,4,0,0);
	int n = (int)wParam;
	switch (n)
	{
	case ctInLen:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,4,0,0);
		break;

	case ctReLen:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,4,0,0);
		break;

	default:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,7,0,0);
		break;
	}

	return 0L;
}
//��갴��
LRESULT CDlgPTZCtrl::OnStgBtnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	CDemoDlg *pDlg = (CDemoDlg *)m_pParent;
	long lRealPlayID = pDlg->m_mapRealPlayView[pDlg->m_nSelViewID];
	m_SelTrack = m_cmbTrack.GetCurSel();
	m_SelPoint = m_cmbPoint.GetCurSel() + 1;
	int n = (int)wParam;
	;//�ж��Ƿ�Ϊ��̨����Ļ�ֱ��return��
	char nSpeed = m_nSpeed;
	switch (n)
	{
	case ctStop://ͣ;
		break;

	case ctUp://��;	
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,7,1,nSpeed);
		break;

	case ctDown://��;
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,7,2,nSpeed);
		break;

	case ctLeft://��;
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,7,3,nSpeed);
		break;

	case ctRight://��;
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,7,4,nSpeed);
		break;

	case ctRU://����;
		break;

	case ctRD://����;
		break;

	case ctLU://����;
		break;

	case ctLD://����;
		break;

	case ctInFoci:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,6,1,nSpeed);
		break;

	case ctReFoci:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,6,2,nSpeed);
		break;

	case ctInLen:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,4,2,nSpeed);
		break;

	case ctReLen:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,4,1,nSpeed);
		break;

	case ctInAperture:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,5,1,nSpeed);
		break;

	case ctReAperture:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,5,2,nSpeed);
		break;

	case ctSetp:
		{
			SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,8,1,m_SelPoint);
			CString str;
			str.Format("����Ԥ�õ� %d�ɹ�",m_SelPoint);
			AfxMessageBox(str);
			m_SelPoint++;
			if(m_SelPoint > 64)
			{
				m_SelPoint = 1;
			}
			m_cmbPoint.SetCurSel(m_SelPoint-1);
			UpdateData(FALSE);
		}
		break;

	case ctGetp:
		{
			SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,8,3,m_SelPoint);
		}
		break;

	case ctDelp:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,8,2,m_SelPoint);
		break;

	case ctDelAllp:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,8,3,78);
		break;	
	case ctCruise:
		SZY_VIEW_PTZControl(pDlg->m_lLoginID,lRealPlayID,8,4,m_SelTrack);
		break;

	case ctInBrush:
		break;

	case ctReBrush:
		break;

	default:
		break;
	}

	return 0L;
}
