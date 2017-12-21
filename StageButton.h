#pragma once


// CStageButton

/////////////////////////////////////////////////////////////////////////////
// CStageButton window
#define WM_STGBTN_LBUTTONDOWN WM_USER + 20
#define WM_STGBTN_LBUTTONUP WM_USER + 21

//按钮控制类型,上 下 左 右 自动,焦聚减,焦聚增,倍长减,倍长增,光圈减,光圈增,雨刷减,雨刷增
enum  ControlType {ctStop,ctUp,ctDown,ctLeft,ctRight,ctRU,ctRD,ctLU,ctLD,ctAuto,ctReFoci,
      ctInFoci,ctReLen,ctInLen,ctReAperture,ctInAperture,ctReBrush,ctInBrush,ctSetp,ctGetp,ctDelp,ctDelAllp,ctCruise,ctNone};
class CStageButton : public CButton
{
//	DECLARE_DYNAMIC(CStageButton)
public:
	ControlType m_Type;
	CStageButton();
	virtual ~CStageButton();
   //{{AFX_MSG(CStageButton) //注释宏 
	DECLARE_MESSAGE_MAP()		
   //}}AFX_MSG //注释宏 
	/*BOOL PreTranslateMessage(MSG* pMsg);*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


