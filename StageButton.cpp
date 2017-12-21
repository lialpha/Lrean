// StageButton.cpp : 实现文件
//

#include "stdafx.h"
#include "StageButton.h"


// CStageButton

//IMPLEMENT_DYNAMIC(CStageButton, CButton)

CStageButton::CStageButton()
{
	m_Type = ctNone;
}

CStageButton::~CStageButton()
{
}

BEGIN_MESSAGE_MAP(CStageButton, CButton)
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



//use PreTranslateMessage to deal with WM_LBUTTONDOWN and  WM_LBUTTONUP
//  BOOL CStageButton::PreTranslateMessage(MSG* pMsg)
//  {
//  	/*if(pMsg->message  == WM_LBUTTONDOWN)
//  	{
//  		::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_STGBTN_LBUTTONDOWN,(int)m_Type,0);
//  		return true;
//  	}
//  	else if(pMsg->message == WM_LBUTTONUP)
//  	{
//  		::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_STGBTN_LBUTTONUP,(int)m_Type,0);
//  		return true;
//  	}*/
//  	//without this, when double click the button, click other point will do the (get the WM_LBUTTONDOWN message), but not get the WM_LBUTTONDOWN message,i don't know why
//  	/*else */if(pMsg->message == WM_LBUTTONDBLCLK)
//  		return true;
//  	return false;
//  
//  }

void CStageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::PostMessage(this->GetParent()->m_hWnd,WM_STGBTN_LBUTTONDOWN,(int)m_Type,0);
	CButton::OnLButtonDown(nFlags, point);
}

void CStageButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::PostMessage(this->GetParent()->m_hWnd,WM_STGBTN_LBUTTONUP,(int)m_Type,0);
	CButton::OnLButtonUp(nFlags, point);
}