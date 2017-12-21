#include "StdAfx.h"
#include "PlayView.h"

CPlayView::CPlayView(void)
{
	m_nHeight = 300;
	m_nWidth = 400;
}

CPlayView::~CPlayView(void)
{
}
BEGIN_MESSAGE_MAP(CPlayView, CStatic)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_COMMAND(ID_CLOSECAMERA, &CPlayView::OnClosecamera)
END_MESSAGE_MAP()

void CPlayView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::PostMessage(this->GetParent()->m_hWnd,WM_VIEW_LBUTTONDOWN,m_nViewID,0);
	CStatic::OnLButtonDown(nFlags, point);
}

void CPlayView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::PostMessage(this->GetParent()->m_hWnd,WM_VIEW_LBUTTONUP,m_nViewID,0);
	CStatic::OnLButtonUp(nFlags, point);
}
void CPlayView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ClientToScreen(&point);
	CMenu subMenu;
	subMenu.CreatePopupMenu();
	subMenu.AppendMenu(MF_BYCOMMAND | MF_ENABLED,ID_CLOSECAMERA,"�ر�");
	

	::GetCursorPos(&point);
	subMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	subMenu.DestroyMenu();
	CStatic::OnRButtonDown(nFlags, point);
}

void CPlayView::OnClosecamera()
{
	// TODO: �ڴ���������������
	::PostMessage(this->GetParent()->m_hWnd,WM_VIEW_CLOSEVIEW,m_nViewID,0);
}
