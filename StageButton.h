#pragma once


// CStageButton

/////////////////////////////////////////////////////////////////////////////
// CStageButton window
#define WM_STGBTN_LBUTTONDOWN WM_USER + 20
#define WM_STGBTN_LBUTTONUP WM_USER + 21

//��ť��������,�� �� �� �� �Զ�,���ۼ�,������,������,������,��Ȧ��,��Ȧ��,��ˢ��,��ˢ��
enum  ControlType {ctStop,ctUp,ctDown,ctLeft,ctRight,ctRU,ctRD,ctLU,ctLD,ctAuto,ctReFoci,
      ctInFoci,ctReLen,ctInLen,ctReAperture,ctInAperture,ctReBrush,ctInBrush,ctSetp,ctGetp,ctDelp,ctDelAllp,ctCruise,ctNone};
class CStageButton : public CButton
{
//	DECLARE_DYNAMIC(CStageButton)
public:
	ControlType m_Type;
	CStageButton();
	virtual ~CStageButton();
   //{{AFX_MSG(CStageButton) //ע�ͺ� 
	DECLARE_MESSAGE_MAP()		
   //}}AFX_MSG //ע�ͺ� 
	/*BOOL PreTranslateMessage(MSG* pMsg);*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


