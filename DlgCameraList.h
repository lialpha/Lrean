#pragma once
#include "DeviceTree.h"
// CDlgCameraList �Ի���

class CDlgCameraList : public CDialog
{
	DECLARE_DYNAMIC(CDlgCameraList)

public:
	CDlgCameraList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCameraList();

// �Ի�������
	enum { IDD = IDD_DLG_CAMERALIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	LPVOID		m_pParent;
	CDeviceTree m_treeCameraList;
	BOOL		m_bDCNode;

	void Init();

	void CreateChildTreeNode(CDeviceTree &treeCtrl,NodeInfo nNodeInfo,int &nEnableNum);
	afx_msg void OnTvnSelchangedTreeCamearlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnNMDblclkTreeCameraDevice(WPARAM wParam,LPARAM lParam);//˫���ڵ�
};
