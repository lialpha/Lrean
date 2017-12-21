#pragma once
#include "Resource.h"

#define WM_VIEW_LBUTTONDOWN WM_USER + 30
#define WM_VIEW_LBUTTONUP WM_USER + 31
#define WM_VIEW_CLOSEVIEW WM_USER + 32
class CPlayView :
	public CStatic
{
public:
	CPlayView(void);
	void InitCtrl(UINT CtrlID,CWnd *pParent)
	{
		m_nViewID = CtrlID;
		SubclassDlgItem(CtrlID,pParent);
	}
	void SetViewID(int nViewID)
	{
		m_nViewID = nViewID;
	}
	void MoveRect(CRect rect)
	{
		if (rect.bottom - rect.top > m_nHeight || rect.bottom - rect.top <= 0)
			return;
		if (rect.right - rect.left > m_nWidth || rect.right - rect.left <= 0)
			return;
		this->MoveWindow(rect);
		this->ShowWindow(SW_SHOW);
	}
	virtual ~CPlayView(void);
public:
	int m_nViewID;
	int m_nHeight;
	int m_nWidth;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClosecamera();
};
