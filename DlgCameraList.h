#pragma once
#include "DeviceTree.h"
// CDlgCameraList 对话框

class CDlgCameraList : public CDialog
{
	DECLARE_DYNAMIC(CDlgCameraList)

public:
	CDlgCameraList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCameraList();

// 对话框数据
	enum { IDD = IDD_DLG_CAMERALIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	LPVOID		m_pParent;
	CDeviceTree m_treeCameraList;
	BOOL		m_bDCNode;

	void Init();

	void CreateChildTreeNode(CDeviceTree &treeCtrl,NodeInfo nNodeInfo,int &nEnableNum);
	afx_msg void OnTvnSelchangedTreeCamearlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnNMDblclkTreeCameraDevice(WPARAM wParam,LPARAM lParam);//双击节点
};
