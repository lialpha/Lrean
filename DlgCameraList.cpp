// DlgCameraList.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo.h"
#include "DlgCameraList.h"
#include "DemoDlg.h"


// CDlgCameraList 对话框

IMPLEMENT_DYNAMIC(CDlgCameraList, CDialog)

CDlgCameraList::CDlgCameraList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCameraList::IDD, pParent)
{
	m_pParent = NULL;
	m_bDCNode = false;
}

CDlgCameraList::~CDlgCameraList()
{
}

void CDlgCameraList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_TREE_CAMEARLIST,m_treeCameraList);
}


BEGIN_MESSAGE_MAP(CDlgCameraList, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CAMEARLIST, &CDlgCameraList::OnTvnSelchangedTreeCamearlist)
	ON_MESSAGE(MSG_NODE_DBLCLICK,&CDlgCameraList::OnNMDblclkTreeCameraDevice)
END_MESSAGE_MAP()


void __stdcall DlgDataCallBack(void *pCall,Video_PACK sVideo)
{
	CDlgCameraList *pDlg = (CDlgCameraList *)pCall;
	
	TRACE("--nWidth:%d,nHeight:%d,nFrameRate:%d,nTS:%d,nDataLen:%d,bKeyFlag:%d\n",
		sVideo.sVideoHead.nWidth,sVideo.sVideoHead.nHeight,sVideo.sVideoHead.nFrameRate,sVideo.sVideoHead.nTS,
		sVideo.sVideoHead.nDataLen,sVideo.sVideoHead.bKeyFlag);	
}

// CDlgCameraList 消息处理程序
void CDlgCameraList::Init()
{
	m_treeCameraList.init();
	m_treeCameraList.DeleteAll();
	m_treeCameraList.SetParentCWnd(this);
	CDemoDlg *pDlg = (CDemoDlg *)m_pParent;
	CString strOtherName,strUserID;
	strOtherName.Format("%s",pDlg->m_sLoginBackData.sClubName);
	strUserID.Format("%s",pDlg->m_sLoginBackData.strUserID);
	HTREEITEM hItem = m_treeCameraList.NewTopZoneToTree(strOtherName,strUserID,TRUE);
	if (hItem == NULL)
		return;
	int nNodeNum = 0;
	SZY_VIEW_GetNodeInfo(pDlg->m_lLoginID,strUserID.GetBuffer(),strUserID.GetBuffer(),nNodeNum);
	SDK_NodeInfo *vNodeInfoList = new SDK_NodeInfo[nNodeNum];
//提供了两种获取节点内容的方法。
	SZY_VIEW_GetNodeValue(pDlg->m_lLoginID,vNodeInfoList,nNodeNum);//第一种
	//for (int i = 0; i < nNodeNum; i ++)
	//{
	//	SZY_VIEW_GetNodeInfoValue(pDlg->m_lLoginID,i,vNodeInfoList[i]);//第二种
	//}
	
	for(int i = 0;i < nNodeNum; i++)
	{
		m_treeCameraList.AddChildZoneToTree(vNodeInfoList[i].sParentID,vNodeInfoList[i].sNodeName,
			vNodeInfoList[i].sNodeID,vNodeInfoList[i].lSubNodeCount,vNodeInfoList[i].nMaxOpenCameraCount,
			vNodeInfoList[i].lSubCameraCount,FALSE,FALSE,vNodeInfoList[i].lActive,hItem);
	}
	delete vNodeInfoList;
	vNodeInfoList = NULL;
	m_treeCameraList.Expand(hItem,TVE_EXPAND); 
}

//生成子树
void CDlgCameraList::CreateChildTreeNode(CDeviceTree &treeCtrl,NodeInfo nNodeInfo,int &nEnableNum)
{
	CDemoDlg *pDlg = (CDemoDlg *)m_pParent;
	TREESXT vCameraList;;
	TREEJIEDIAN vNodeList;
	BOOL bIsFirst = TRUE;
	if (nNodeInfo.m_vSubCameraIDList.size() > 0)
		bIsFirst = FALSE;
	if(nNodeInfo.nSubNodeCount > 0)					//有子节点
	{
		int nNodeNum = 0;
		SZY_VIEW_GetNodeInfo(pDlg->m_lLoginID,pDlg->m_sLoginBackData.strUserID,nNodeInfo.sNodeID.GetBuffer(),nNodeNum);
		SDK_NodeInfo *vNodeInfoList = new SDK_NodeInfo[nNodeNum];
		SZY_VIEW_GetNodeValue(pDlg->m_lLoginID,vNodeInfoList,nNodeNum);
		for (int i = 0; i < nNodeNum; i ++)
		{
			TreeJiedian sTempNode;
			ZeroMemory(sTempNode.JIeDian_id,50);
			memcpy(sTempNode.JIeDian_id,(*(vNodeInfoList + i)).sNodeID,strlen((*(vNodeInfoList + i)).sNodeID));
			ZeroMemory(sTempNode.JIeDian_name,50);
			memcpy(sTempNode.JIeDian_name,(*(vNodeInfoList + i)).sNodeName,strlen((*(vNodeInfoList + i)).sNodeName));
			ZeroMemory(sTempNode.Parent_id,50);
			memcpy(sTempNode.Parent_id,(*(vNodeInfoList + i)).sParentID,strlen((*(vNodeInfoList + i)).sParentID));

			sTempNode.Active = (*(vNodeInfoList + i)).lActive;
			sTempNode.SXT_Count = (*(vNodeInfoList + i)).lSubCameraCount;
			sTempNode.JieDian_Count = (*(vNodeInfoList + i)).lSubNodeCount;
			sTempNode.nMaxOpenCameraCount = (*(vNodeInfoList + i)).nMaxOpenCameraCount;
			vNodeList.push_back(sTempNode);
		}
		
		delete vNodeInfoList;
		vNodeInfoList = NULL;
	}
	if(nNodeInfo.nSubCameraCount > 0)					//子节点有摄像头
	{
		int nCameraNum = 0;
		SZY_VIEW_GetCameraInfo(pDlg->m_lLoginID,pDlg->m_sLoginBackData.strUserID,nNodeInfo.sNodeID.GetBuffer(),nCameraNum);
	//	TreeSXT  *vCameraInfoList = new TreeSXT[nCameraNum];
		SDK_CameraInfo *vCameraInfoList = new SDK_CameraInfo[nCameraNum];
		SZY_VIEW_GetCameraValue(pDlg->m_lLoginID,vCameraInfoList,nCameraNum);
		for (int i = 0; i < nCameraNum; i ++)
		{
			TreeSXT sCameraInfo;
			ZeroMemory(sCameraInfo.SXT_id,50);
			memcpy(sCameraInfo.SXT_id,(*(vCameraInfoList + i)).sCameraID,strlen((*(vCameraInfoList + i)).sCameraID));
			ZeroMemory(sCameraInfo.SXT_Name,50);
			memcpy(sCameraInfo.SXT_Name,(*(vCameraInfoList + i)).sCameraName,strlen((*(vCameraInfoList + i)).sCameraName));
			ZeroMemory(sCameraInfo.JieDian_id,50);
			memcpy(sCameraInfo.JieDian_id,(*(vCameraInfoList + i)).sNodeID,strlen((*(vCameraInfoList + i)).sNodeID));
			ZeroMemory(sCameraInfo.Server_ip,20);
			memcpy(sCameraInfo.Server_ip,(*(vCameraInfoList + i)).sRemtoeIP,strlen((*(vCameraInfoList + i)).sRemtoeIP));
			sCameraInfo.atTerm = (*(vCameraInfoList + i)).IsLate;
			sCameraInfo.Active = (*(vCameraInfoList + i)).lActive;
			sCameraInfo.AudioSwitch = (*(vCameraInfoList + i)).lAudioFlag;
			sCameraInfo.acquisitionType = (*(vCameraInfoList + i)).lDeviceType;
			sCameraInfo.YunTai_Flag = (*(vCameraInfoList + i)).lIsPTZ;
			sCameraInfo.Server_Port = (*(vCameraInfoList + i)).lRemotePort;
			sCameraInfo.ZhiBo_Flag = 0;
			vCameraList.push_back(sCameraInfo);
		}
		delete vCameraInfoList;
		vCameraInfoList = NULL;
	}
	nEnableNum = treeCtrl.CreateNodeTree(nNodeInfo,vNodeList,vCameraList);
}


void CDlgCameraList::OnTvnSelchangedTreeCamearlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	CDemoDlg *pDlg = (CDemoDlg *)m_pParent;
	NodeInfo nNodeInfo;
	BOOL ret = m_treeCameraList.GetItemNodeInfo(hItem,nNodeInfo);
	int nEnableNum = 0;
	if(ret == FALSE)
		return;
	else
	{
		if ((nNodeInfo.nSubNodeCount >0 || nNodeInfo.nSubCameraCount >0) && !m_treeCameraList.FindChildItem(nNodeInfo))
			CreateChildTreeNode(m_treeCameraList,nNodeInfo,nEnableNum);
		if (nNodeInfo.IsSXT)
		{
			TreeSXT sCamera;
			m_treeCameraList.GetItemSXTInfo(nNodeInfo.sNodeID,sCamera);
			pDlg->UpdateNowCamera(sCamera);
		}
	}
	*pResult = 0;
}

//双击节点
LRESULT CDlgCameraList::OnNMDblclkTreeCameraDevice(WPARAM wParam,LPARAM lParam)
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bDCNode)
		return 0;
	m_bDCNode = TRUE;
	NodeInfo  *pNode = (NodeInfo *)lParam;
	NodeInfo nNodeInfo;
	nNodeInfo.CreateNode(pNode);
	if (nNodeInfo.IsSXT)
	{
		TreeSXT sCamera;
		m_treeCameraList.GetItemSXTInfo(nNodeInfo.sNodeID,sCamera);
		CDemoDlg *pDlg = (CDemoDlg *)m_pParent;
		int nSelViewID = pDlg->m_nSelViewID;
		pDlg->CloseView(nSelViewID);
		
		if (nSelViewID == IDC_STATIC_VIEW2) //测试数据回调函数
		{
			long lRealPlayID = SZY_VIEW_InitRealPlayEX(pDlg->m_lLoginID,(void *)DlgDataCallBack,SZY_H264,this);

			if (lRealPlayID != 0)
				pDlg->m_mapRealPlayView[nSelViewID] = lRealPlayID;
			if(!SZY_VIEW_RealPlay(pDlg->m_lLoginID,lRealPlayID,sCamera.Server_ip,sCamera.Server_Port,NULL,sCamera.SXT_id))
				pDlg->CloseView(nSelViewID);
			else
			{
				if(!SZY_VIEW_OpenAudioListen(pDlg->m_lLoginID,lRealPlayID))
					AfxMessageBox("打开音频播放失败");
			}
		}
		else
		{
			long lRealPlayID = SZY_VIEW_InitRealPlay(pDlg->m_lLoginID,NULL,NULL);

			if (lRealPlayID != 0)
				pDlg->m_mapRealPlayView[nSelViewID] = lRealPlayID;
			if(!SZY_VIEW_RealPlay(pDlg->m_lLoginID,lRealPlayID,sCamera.Server_ip,sCamera.Server_Port,pDlg->m_mapViewWnd[nSelViewID]->m_hWnd,sCamera.SXT_id))
				pDlg->CloseView(nSelViewID);
			else
			{
				if(!SZY_VIEW_OpenAudioListen(pDlg->m_lLoginID,lRealPlayID))
					AfxMessageBox("打开音频播放失败");
			}
		}
	}
	m_bDCNode = FALSE;
	return 1;
}
