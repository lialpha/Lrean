#include "StdAfx.h"
#include "Resource.h"
#include "DeviceTree.h"

CDeviceTree::CDeviceTree(void)
{
	::InitializeCriticalSection(&m_cs);
	m_pParent = NULL;
	m_sFindNodeName.Empty();
	m_nCount = 0;
	m_StartFindNode = NULL;
}

CDeviceTree::~CDeviceTree(void)
{
	m_mapTree.clear();
	m_mapSXT.clear();
}
void CDeviceTree::init()
{
	m_TreeImage.DeleteImageList();
	m_TreeImage.Create ( IDB_BITMAP_TREE_IMAGE,16,1,ILC_COLOR8);
	SetImageList ( &m_TreeImage,TVSIL_NORMAL);
	m_SelCol = RGB(0,0,255);//选中颜色设置：蓝色
	m_InitCol = RGB(0,0,0);//获取初始化字体颜色:黑色
	SetTextColor (m_InitCol);
	//	SetItemHeight(20);
	SetBkColor(RGB(255,255,255));//设置m_mytree的背景
}

void CDeviceTree::InitTopTree(CString sUserName,CString sClubID,TREEJIEDIAN TreeList)
{
	HTREEITEM hItem = NewTopZoneToTree(sUserName,sClubID,TRUE);
	if (hItem == NULL)
		return;
	SetItemColor(hItem,m_InitCol);
	SetItemBold(hItem,FALSE);
// 	TREEJIEDIAN TreeList;
// 	pDlg->m_adoHandle.GetNodeId(sClubID,sClubID,TreeList);
	TREEJIEDIANIt it = TreeList.begin();
	for(;it != TreeList.end(); it++)
	{
		AddChildZoneToTree(it->Parent_id,it->JIeDian_name,it->JIeDian_id,it->JieDian_Count,it->nMaxOpenCameraCount,it->SXT_Count,FALSE,FALSE,it->Active,hItem);
	}
	Expand(hItem,TVE_EXPAND);
}

void CDeviceTree::UpdateTreeNode(NodeInfo nNodeInfo,int index)
{
	for(map<HTREEITEM, NodeInfo>::iterator it=m_mapTree.begin();it!=m_mapTree.end();it++)
	{
		if(it->second.sNodeID.Trim() == nNodeInfo.sNodeID.Trim())
		{
			if (index < 0)
			{
				it->second.m_vOpenCameraIDList = nNodeInfo.m_vOpenCameraIDList;
				it->second.m_vSubCameraIDList = nNodeInfo.m_vSubCameraIDList;
			}
			else
				it->second.nCameraIndex = nNodeInfo.nCameraIndex;
			break;
		}
	}
}

int CDeviceTree::CreateNodeTree(NodeInfo nNodeInfo,TREEJIEDIAN TreeList,TREESXT TreeSXT)
{

	int count = 0;
	if(nNodeInfo.nSubNodeCount > 0)					//有子节点
	{
		TREEJIEDIANIt it = TreeList.begin();
		for(;it != TreeList.end(); it++)
		{
			AddChildZoneToTree(it->Parent_id,it->JIeDian_name,it->JIeDian_id,it->JieDian_Count,it->nMaxOpenCameraCount,
				it->SXT_Count,FALSE,FALSE,it->Active,nNodeInfo.hTreeItem);
			if (it->Active > 0)
				count ++;
		}
	}
	if(nNodeInfo.nSubCameraCount > 0)					//子节点有摄像头
	{	
		TREESXTIt it = TreeSXT.begin();
		for(;it != TreeSXT.end(); it++)
		{
			int res = AddChildZoneToTree(it->JieDian_id,it->SXT_Name,it->SXT_id,0,0,
				0,TRUE,it->atTerm,it->Active,nNodeInfo.hTreeItem);
			NodeInfo tempNode;
			GetItemNodeInfo(it->SXT_id,tempNode);
			if (it->atTerm)
				SetItemImage(tempNode.hTreeItem,9,9);
			if(!(it->atTerm) && it->Active)
				count ++;
			AddSXTToTree(tempNode.hTreeItem,(*it));
		}
	}
	if (nNodeInfo.nPageCount == 0)	
		return count;
	else
		return nNodeInfo.nPageCount;
}

HTREEITEM  CDeviceTree::NewTopZoneToTree(CString  sNodeName,CString  sNodeID,BOOL bHasSub)
{
	DWORD data =0;
	HTREEITEM htreeitem = InsertItem(sNodeName,NULL);
	
	if(NULL!=htreeitem)
	{ 
		SetItemColor(htreeitem,m_InitCol);
		SetItemBold(htreeitem,FALSE);
		NodeInfo sNodeInfo;
		sNodeInfo.sNodeName = sNodeName;
		sNodeInfo.nSubNodeCount = 0;
		sNodeInfo.IsSXT = FALSE;
		sNodeInfo.nSubCameraCount = 0;
		sNodeInfo.sNodeID = sNodeID;
		sNodeInfo.hTreeItem = htreeitem;
		sNodeInfo.strParentNodeID = "";
		m_mapTree[htreeitem] = sNodeInfo;
		
		SetItemImage(htreeitem,0,1);
		return htreeitem;
	}
	return 0;
}

//修改节点信息，当hItem不为空时表示：当前节点移动到其他节点下
HTREEITEM  CDeviceTree::AlterItemNodeInfo(CString nodeId,CString parentNodeId,CString strNewNodeName,int nPageCount,HTREEITEM hItem)
{
	HTREEITEM retTreeItem;
	for(map<HTREEITEM, NodeInfo>::iterator it=m_mapTree.begin();it!=m_mapTree.end();it++)
	{
		if(nodeId.Trim().Compare((it->second.sNodeID)) == 0)	
		{
			if (parentNodeId != "")	
				it->second.strParentNodeID = parentNodeId;
			if (strNewNodeName != "")
				it->second.sNodeName = strNewNodeName;
			if(hItem != NULL)
			{
				it->second.hTreeItem = InsertItem(strNewNodeName,hItem);
				m_mapTree[it->second.hTreeItem] = it->second;
			}
			it->second.nPageCount = nPageCount;
			retTreeItem = it->second.hTreeItem;
		}
	}
	return retTreeItem;
}
void CDeviceTree::AddNodeCount(CString nodeId)
{
	for(map<HTREEITEM, NodeInfo>::iterator it=m_mapTree.begin();it!=m_mapTree.end();it++)
	{
		if(nodeId.Trim().Compare((it->second.sNodeID)) == 0)
		{
			it->second.nSubNodeCount ++;
			break;
		}
	}
}

void CDeviceTree::SubtractNodeCount(CString nodeId)
{
	for(map<HTREEITEM, NodeInfo>::iterator it=m_mapTree.begin();it!=m_mapTree.end();it++)
	{
		if(nodeId.Trim().Compare((it->second.sNodeID)) == 0)
		{
			if (it->second.nSubNodeCount > 0)
				it->second.nSubNodeCount --;
			break;
		}
	}
}

void CDeviceTree::SubtractCameraCount(CString nodeId)
{
	for(map<HTREEITEM, NodeInfo>::iterator it=m_mapTree.begin();it!=m_mapTree.end();it++)
	{
		if(nodeId.Trim().Compare((it->second.sNodeID)) == 0)
		{
			if (it->second.nSubCameraCount > 0)
				it->second.nSubCameraCount --;
			break;
		}
	}
}

int  CDeviceTree::AddChildZoneToTree(CString strParentNodeID,CString  sNodeName,CString  sNodeID,int nSubNodeCount,int nMaxOpenCameraCount,
										   int  nSubCameraCount,BOOL IsSXT,BOOL atTerm,BOOL bActive,HTREEITEM hItem) 
{
	for(map<HTREEITEM, NodeInfo>::iterator it=m_mapTree.begin();it!=m_mapTree.end();it++)
	{
		if(sNodeID.Trim().Compare((it->second.sNodeID)) == 0 && (!it->second.strParentNodeID.IsEmpty()))
			return -1;
	}
	CString strCameraName;
	if(atTerm)
		strCameraName = sNodeName.Trim() + _T("\(已到期\)");
	else
		strCameraName = sNodeName.Trim();
	HTREEITEM htreeitem = InsertItem(strCameraName,hItem);
	if(NULL!=htreeitem)
	{ 
		SetItemColor(htreeitem,m_InitCol);
		SetItemBold(htreeitem,FALSE);
		NodeInfo sNodeInfo;
		sNodeInfo.Init();
		sNodeInfo.sNodeName = sNodeName;
		sNodeInfo.sShowName = strCameraName;
		sNodeInfo.nSubNodeCount = nSubNodeCount;
		sNodeInfo.IsSXT = IsSXT;
		sNodeInfo.nSubCameraCount = nSubCameraCount;
		sNodeInfo.sNodeID = sNodeID.Trim();

		sNodeInfo.hTreeItem = htreeitem;
		sNodeInfo.strParentNodeID = strParentNodeID.Trim();
		sNodeInfo.nPageCount = nMaxOpenCameraCount;
		m_mapTree[htreeitem] = sNodeInfo;


		if(IsSXT)
		{
			if(bActive)				//在线
				SetItemImage(htreeitem,2,3);
			else
				SetItemImage(htreeitem,7,8);
			NodeInfo tempNode;
			GetItemNodeInfo(strParentNodeID,tempNode);
			tempNode.m_vSubCameraIDList.push_back(sNodeID);
			UpdateTreeNode(tempNode);
		}
		else
		{
			if(bActive)				//在线
				SetItemImage(htreeitem,0,1);
			else
				SetItemImage(htreeitem,5,6);			
		}
		return 1;
	}	
	UpdateData();
	return 0;
}
BOOL CDeviceTree::GetItemNodeInfo(HTREEITEM hItem,NodeInfo &sNodeInfo)
{
	if(m_mapTree.find(hItem) != m_mapTree.end())
	{
		sNodeInfo = m_mapTree[hItem];
		return TRUE;
	}
	return FALSE;
}

BOOL CDeviceTree::GetItemNodeInfo(CString nodeId,NodeInfo &sNodeInfo)
{
	
	for(map<HTREEITEM, NodeInfo>::iterator it=m_mapTree.begin();it!=m_mapTree.end();it++)
	{
		if(nodeId.Trim().Compare((it->second.sNodeID)) == 0)
		{
			sNodeInfo = it->second;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDeviceTree::GetItemSXTInfo(HTREEITEM hItem,TreeSXT  &sTreeSXT)
{
	if(m_mapSXT.find(hItem) != m_mapSXT.end())
	{
		sTreeSXT = m_mapSXT[hItem];
		return TRUE;
	}
	return FALSE;
}

BOOL CDeviceTree::GetItemSXTInfo(CString sCameraID,TreeSXT &sTreeSXT)
{
	for(map<HTREEITEM, TreeSXT>::iterator it=m_mapSXT.begin();it!=m_mapSXT.end();it++)
	{
		if (strcmp(it->second.SXT_id,sCameraID.GetBuffer()) == 0)
		{
			sTreeSXT = it->second;
			return TRUE;
		}
	}
	return FALSE;
}

//获取该节点下的所有摄像头集合
int	 CDeviceTree::GetNodeSubItemSXT(CString sNodeID,TREESXT &vTreeSXT)
{
	vTreeSXT.clear();
	for(map<HTREEITEM, NodeInfo>::iterator it=m_mapTree.begin();it!=m_mapTree.end();it++)
	{
		if (it->second.strParentNodeID.Trim() == sNodeID && it->second.IsSXT)
		{
			vTreeSXT.push_back(m_mapSXT[it->second.hTreeItem]);
		}
	}
	return  vTreeSXT.size();
}

BOOL CDeviceTree::GetItemSXTInfo(CString sCameraID,NodeInfo &sNodeInfo)
{
	return FALSE;
}

void CDeviceTree::DeleteAll()
{
	
	m_mapTree.clear();
	m_mapSXT.clear();
	DeleteAllItems();
}

void CDeviceTree::DeleteNodeForMapTree(CString nodeID)
{
	for(map<HTREEITEM, NodeInfo>::iterator it=m_mapTree.begin();it!=m_mapTree.end();it++)
	{
		if(nodeID.Trim().Compare((it->second.sNodeID)) == 0)
		{
			NodeInfo temp;
			GetItemNodeInfo(nodeID,temp);
			DeleteItem(temp.hTreeItem);
			m_mapTree.erase(it);
			break;
		}
	}
}

void CDeviceTree::DeleteNodeForMapSXT(CString nodeID)
{

	map<HTREEITEM, TreeSXT>::iterator it=m_mapSXT.begin();
	while (it != m_mapSXT.end())
	{
		if(nodeID.Trim().Compare((it->second.JieDian_id)) == 0)
		{
			
			NodeInfo temp;
			GetItemNodeInfo(it->second.SXT_id,temp);
			DeleteItem(temp.hTreeItem);
			it = m_mapSXT.erase(it);
		}
		else
			it ++;
	}
}

void CDeviceTree::DeleteCameraForMapSXT(CString sCameraId)
{
	map<HTREEITEM, TreeSXT>::iterator it=m_mapSXT.begin();
	while (it != m_mapSXT.end())
	{
		if(sCameraId.Trim().Compare((it->second.SXT_id)) == 0)
		{
			
			NodeInfo temp;
			GetItemNodeInfo(it->second.SXT_id,temp);
			DeleteItem(temp.hTreeItem);
			m_mapSXT.erase(it);
			break;
		}
		else
			it ++;
	}
}

//删除目录节点
void CDeviceTree::DeleteNodeForTree(CString nodeID)
{
	NodeInfo sNodeInfo;
	GetItemNodeInfo(nodeID,sNodeInfo);//获取节点信息
	SubtractNodeCount(sNodeInfo.strParentNodeID);//父节点下的子节点数减一。

	if (sNodeInfo.nSubCameraCount > 0)
		DeleteNodeForMapSXT(nodeID);//删除摄像头
	DeleteNodeForMapTree(nodeID);//删除目录节点
}

void CDeviceTree::DeleteCameraNodeForTree(CString nodeID,CString sCameraId)
{
	SubtractCameraCount(nodeID);//所在目录节点的子摄像头数减一。
	DeleteCameraForMapSXT(sCameraId);//删除摄像头
}

void CDeviceTree::UpdateCheck(HTREEITEM hItem,BOOL check)
{
	if (hItem == NULL)
		return;
	HTREEITEM childTree = GetChildItem(hItem);
	while (childTree != NULL)
	{
		UpdateCheck(childTree,check);
		SetCheck(childTree,GetCheck(hItem)^check);
		childTree = GetNextItem(childTree,TVGN_NEXT);
	}
}

void CDeviceTree::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		cf.color = (COLORREF)-1;
	cf.logfont = logfont;
	m_mapColorFont[hItem] = cf;
}

void CDeviceTree::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
	SetItemState( hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD );
}

void CDeviceTree::SetItemColor(HTREEITEM hItem, COLORREF color)
{
	if (hItem == NULL)
		return;
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		cf.logfont.lfFaceName[0] = '\0';
	cf.color = color;
	m_mapColorFont[hItem] = cf;
	if (color == m_InitCol)
		SetItemBold(hItem,FALSE);
	else
		SetItemBold(hItem,TRUE);
	
}

BOOL CDeviceTree::GetItemFont(HTREEITEM hItem, LOGFONT * plogfont)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		return FALSE;
	if( cf.logfont.lfFaceName[0] == '\0' ) 
		return FALSE;
	*plogfont = cf.logfont;
	return TRUE;
}

BOOL CDeviceTree::GetItemBold(HTREEITEM hItem)
{
	return GetItemState( hItem, TVIS_BOLD ) & TVIS_BOLD;
}

COLORREF CDeviceTree::GetItemColor(HTREEITEM hItem)
{
	// Returns (COLORREF)-1 if color was not set
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		return (COLORREF)-1;
	return cf.color;
}

void CDeviceTree::MakePaint()
{
	OnPaint();
}

BEGIN_MESSAGE_MAP(CDeviceTree, CTreeCtrl)
ON_WM_PAINT()
ON_NOTIFY_REFLECT(NM_CLICK, &CDeviceTree::OnNMClick)
ON_NOTIFY_REFLECT(NM_DBLCLK, &CDeviceTree::OnNMDblclk)
END_MESSAGE_MAP()

void CDeviceTree::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CTreeCtrl::OnPaint()
	CDC memDC;
	memDC.CreateCompatibleDC( &dc );
	CRect rcClip, rcClient;
	dc.GetClipBox( &rcClip );
	GetClientRect(&rcClient);
	// Select a compatible bitmap into the memory DC
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
	memDC.SelectObject( &bitmap );
	// Set clip region to be same as that in paint DC
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcClip );
	memDC.SelectClipRgn(&rgn);
	rgn.DeleteObject();
	// First let the control do its default drawing.
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
	HTREEITEM hItem = GetFirstVisibleItem();
	int n = GetVisibleCount()+1;
	while( hItem && n--)
	{
		CRect rect;
		// Do not meddle with selected items or drop highlighted items
		UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;
		Color_Font cf;
		if ( !(GetItemState( hItem, selflag ) & selflag )
			&& m_mapColorFont.Lookup( hItem, cf ))
		{
			CFont *pFontDC;
			CFont fontDC;
			LOGFONT logfont;
			if( cf.logfont.lfFaceName[0] != '\0' )
			{
				logfont = cf.logfont;
			}
			else
			{
				// No font specified, so use window font
				CFont *pFont = GetFont();
				pFont->GetLogFont( &logfont );
			}
			if( GetItemBold( hItem ) )
				logfont.lfWeight = 700;
			fontDC.CreateFontIndirect( &logfont );
			pFontDC = memDC.SelectObject( &fontDC );
			if( cf.color != (COLORREF)-1 )
				memDC.SetTextColor( cf.color );
			CString sItem = GetItemText( hItem );
			GetItemRect( hItem, &rect, TRUE );
			memDC.SetBkColor( GetSysColor( COLOR_WINDOW ) );
			memDC.TextOut( rect.left+2, rect.top+1, sItem );
			memDC.SelectObject( pFontDC );
		}
		hItem = GetNextVisibleItem( hItem );
	}
	dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC,
		rcClip.left, rcClip.top, SRCCOPY );
}

//鼠标左键点击事件
void CDeviceTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NodeInfo nowNode;
	UINT uFlag;
	CPoint pt;
	CRect rc;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	GetWindowRect(rc);
	ScreenToClient(rc);
	pt.x -= rc.left;
	pt.y -= (rc.top + 2);
	HTREEITEM hItem=HitTest(pt,&uFlag);
	GetItemNodeInfo(hItem,nowNode);
	BOOL bCheck;
	if ((hItem != NULL) && (TVHT_ONITEMSTATEICON & uFlag))
	{
		::SendMessage(((CDialog *)m_pParent)->m_hWnd,MSG_NMCHECKNODE,(WPARAM)this,LPARAM(&nowNode));
		bCheck = GetCheck(hItem);
		SetCheckState(hItem,!bCheck);
		CheckSubItem(hItem,!bCheck);
	}
	if (m_pParent != NULL && TVHT_ONITEMLABEL & uFlag)
	{
		::SendMessage(((CDialog *)m_pParent)->m_hWnd,MSG_NMCLICK,(WPARAM)this,LPARAM(&nowNode));
	}
	
	*pResult = 0;
}

void CDeviceTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NodeInfo nowNode;
	UINT uFlag;
	CPoint pt;
	CRect rc;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	GetWindowRect(rc);
	ScreenToClient(rc);
	pt.x -= rc.left;
	pt.y -= (rc.top + 2);
	HTREEITEM hItem=HitTest(pt,&uFlag);
	GetItemNodeInfo(hItem,nowNode);
	if (hItem != NULL)
	{
		if (m_pParent != NULL && TVHT_ONITEMLABEL & uFlag)
		{
			::SendMessage(((CDialog *)m_pParent)->m_hWnd,MSG_NODE_DBLCLICK,(WPARAM)this,LPARAM(&nowNode));
		}
	}
	*pResult = TRUE;//为true表示双击节点不响应展开或收起树节点操作。false 则相反
}

// 勾选父节点时，所有子节点也勾选
void CDeviceTree::CheckSubItem(HTREEITEM hNowTreeItem, BOOL bCheck)
{
	HTREEITEM hCur,hNext;
	hCur = GetChildItem(hNowTreeItem);

	while(hCur != NULL)
	{
		hNext = GetNextSiblingItem(hCur);
		SetCheck(hCur,bCheck);
		SetCheckState(hCur,bCheck);
		CheckSubItem(hCur,bCheck);
		hCur = hNext;
	}
}

//初始化子树为空
void CDeviceTree::ClearSubTree()
{
	m_mSubTree.clear();
}

//初始化所有节点的勾选状态为没选中状态

void CDeviceTree::InitTreeNodeNoCheck()
{
	map<HTREEITEM, NodeInfo>::iterator it=m_mapTree.begin();
	while (it != m_mapTree.end())
	{
		it->second.Check = FALSE;
		SetCheck(it->second.hTreeItem,FALSE);
		it ++;
	}
}

//显示子树到树上，用勾选框的状态显示
void CDeviceTree::ShowSubTreeNodeCheck()
{
	InitTreeNodeNoCheck();
	NodeInfo tempNode;
	for (int i = 0; i < m_mSubTree.size(); i ++)
	{
		GetItemNodeInfo(m_mSubTree[i].sNodeID,tempNode);
		SetCheckState(tempNode.hTreeItem,TRUE);
		SetCheck(tempNode.hTreeItem,TRUE);
	}
}

//设置知道节点的勾选框状态
void CDeviceTree::SetCheckState(HTREEITEM hNowTreeItem, BOOL bCheck)
{
	m_mapTree[hNowTreeItem].Check = bCheck;
}

//从当前树中获取勾选的子树
BOOL CDeviceTree::GetCheckNodeTree(HTREEITEM hNowTreeItem)
{
	NodeInfo tempNode;
	GetItemNodeInfo(hNowTreeItem,tempNode);
	if (tempNode.IsSXT)
	{
		if(tempNode.Check)
		{
			m_mSubTree.push_back(tempNode);
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		HTREEITEM hCur,hNext;
		int count = 0;
		hCur = GetChildItem(hNowTreeItem);
		if (hCur == NULL)
			return FALSE;//节点底下没有子节点或子摄像头
		while(hCur != NULL)
		{
			hNext = GetNextSiblingItem(hCur);
			if(GetCheckNodeTree(hCur))
				count ++;
			hCur = hNext;
		}
		if (count > 0)
		{
			m_mSubTree.push_back(tempNode);
			return TRUE;
		}
		else
			return FALSE;
	}
}

//把勾选的子树转成字符串
void CDeviceTree::TreeToStr(CString &str)
{
	CString sTemp;
	str.Empty();
	for(int i = 0; i < m_mSubTree.size(); i++)
	{	
		sTemp.Format("%d",m_mSubTree[i].IsSXT);
		str = str + m_mSubTree[i].strParentNodeID + _T(",") + m_mSubTree[i].sNodeID + _T(",") + sTemp + _T(";");
	}
}

//从字符串转成子树
void CDeviceTree::StrToTree(CString str)
{
	m_mSubTree.clear();
	NodeInfo tempNode;
	CString sTemp;
	CString sSXTTag;
	int i = 0;
	AfxExtractSubString(sTemp,str,i,';');
	i ++;
	while(!sTemp.Trim().IsEmpty())
	{
		AfxExtractSubString(tempNode.strParentNodeID,sTemp,0,',');
		AfxExtractSubString(tempNode.sNodeID,sTemp,1,',');
		AfxExtractSubString(sSXTTag,sTemp,2,',');
		tempNode.IsSXT = atoi(sSXTTag);
		m_mSubTree.push_back(tempNode);
		AfxExtractSubString(sTemp,str,i,';');
		i ++;
	}
}
// 获取父节点下一级的所有节点集合
void CDeviceTree::GetNodeListForParent(vector<NodeInfo> &vNodeTree,CString parentNodeID)
{
	vNodeTree.clear();
	for (int i = 0; i < m_mSubTree.size(); i++)
	{
		if (parentNodeID == m_mSubTree[i].strParentNodeID.Trim())
		{
			vNodeTree.push_back(m_mSubTree[i]);
		}
	}
}
//组装要向服务器请求节点信息的节点ID列表字符串
void CDeviceTree::NodeIDListToStr(CString &str,vector<NodeInfo> vNodeTree)
{
	str.Empty();
	for (int i = 0; i < vNodeTree.size(); i++)
	{
		str = str + vNodeTree[i].sNodeID + _T(",");
	}
}

//获取子树内容
void CDeviceTree::GetSubTree(vector<NodeInfo> &vSubTree)
{
	vSubTree = m_mSubTree;
}
//组装成访问数据库需要的字符串
void CDeviceTree::NodeListToStr(vector<NodeInfo> vSubTree,CString &sSqlStr)
{
	sSqlStr.Empty();
	CString sTemp;
	if (vSubTree.size() > 0)
		sSqlStr = _T("''") + vSubTree[0].sNodeID.Trim() + _T("''");
	for (int i = 1; i < vSubTree.size(); i++)
	{	
		sSqlStr = sSqlStr + _T(",");
		sSqlStr = sSqlStr + _T("''") + vSubTree[i].sNodeID.Trim() + _T("''");
	}
}

//获取子树中的节点数
int  CDeviceTree::GetSubTreeSize()
{
	return m_mSubTree.size();
}

//从一棵已知的树上拷贝一棵树
BOOL CDeviceTree::CopyData_tree2tree(CDeviceTree *treeFrom,HTREEITEM itemFrom,HTREEITEM itemTo) 
{ 
	if(treeFrom == NULL) 
		return   false; 
	if(itemTo == TVI_ROOT)
		this->DeleteAllItems(); 
	HTREEITEM treeFromItem; 
	if(itemFrom == TVI_ROOT)
		treeFromItem = treeFrom->GetRootItem();
	else
		treeFromItem = treeFrom->GetChildItem(itemFrom);

	while(treeFromItem)
	{ 
		int nImageOne,nImageTwo;
		CString strText = treeFrom->GetItemText(treeFromItem); 
		WORD data = treeFrom->GetItemData(treeFromItem); 
		//如果还要拷贝其他数据，在这里加！ 
		HTREEITEM itemToTmp  = InsertItem(strText,itemTo); 
		m_mapTree[itemToTmp] = treeFrom->m_mapTree[treeFromItem];
		m_mapTree[itemToTmp].hTreeItem = itemToTmp;
		m_mapSXT[itemToTmp] = treeFrom->m_mapSXT[treeFromItem];
		treeFrom->GetItemImage(treeFromItem,nImageOne,nImageTwo);
		SetItemImage(itemToTmp,nImageOne,nImageTwo);
		SetItemData(itemToTmp,data); 
		HTREEITEM itemFromTmp = treeFrom->GetChildItem(treeFromItem); 
		if(itemFromTmp)
			CopyData_tree2tree(treeFrom,treeFromItem,itemToTmp);
		treeFromItem = treeFrom->GetNextItem(treeFromItem,TVGN_NEXT); 
	} 
	return true; 
}

//拷贝树
BOOL CDeviceTree::CopyTree(CDeviceTree * treeFrom,HTREEITEM itemFrom,HTREEITEM itemTo)
{
	treeFrom->GetSubTree(m_mSubTree);
	return CopyData_tree2tree(treeFrom,itemFrom,itemTo);
}

BOOL CDeviceTree::AbleSeeCamera(int nUserType,CString sCameraID)
{
	NodeInfo node;
	TreeSXT camera;
	GetItemNodeInfo(sCameraID,node);
	GetItemSXTInfo(node.hTreeItem,camera);
	if (!camera.Active)
		return FALSE;
	switch (nUserType)
	{
	case SUPER_USER:
		{
			return TRUE;
			break;
		}
	case SUBUSER:
	case MAINUSER:
		{
			return !camera.atTerm;
		}
	}
	return TRUE;
}

void CDeviceTree::ChangeOpenCameraIndex(CString sNodeID,CString sCameraID)
{
	NodeInfo sParentNode;
	EnterCriticalSection(&m_cs);
	GetItemNodeInfo(sNodeID,sParentNode);
	for (int i = 0; i < sParentNode.m_vOpenCameraIDList.size(); i ++)
	{
		if (sParentNode.m_vOpenCameraIDList[i].strCameraID)
		{
			SCameraInfo tempCamera;
			tempCamera = sParentNode.m_vOpenCameraIDList[i];
			sParentNode.m_vOpenCameraIDList[i] = sParentNode.m_vOpenCameraIDList[0];
			sParentNode.m_vOpenCameraIDList[0] = tempCamera;
			break;
		}
	}
	UpdateTreeNode(sParentNode);
	LeaveCriticalSection(&m_cs);
}

//关闭摄像头连接
void CDeviceTree::StopCamera(CString sCameraID)
{
	NodeInfo sCameraNode,sParentNode;
	EnterCriticalSection(&m_cs);
	GetItemNodeInfo(sCameraID,sCameraNode);
	GetItemNodeInfo(sCameraNode.strParentNodeID,sParentNode);
	for (int i = 0; i < sParentNode.m_vOpenCameraIDList.size(); i ++)
	{
		if (sParentNode.m_vOpenCameraIDList[i].strCameraID == sCameraID && (--(sParentNode.m_vOpenCameraIDList[i].nCount)) == 0)
		{
			sParentNode.m_vOpenCameraIDList.erase(sParentNode.m_vOpenCameraIDList.begin() + i);
			SetItemColor(sCameraNode.hTreeItem,m_InitCol);
		//	SetItemText(sCameraNode.hTreeItem,sCameraNode.sShowName);
		//	SetItemBold(sCameraNode.hTreeItem,FALSE);
			break;
		}
	}
	UpdateTreeNode(sParentNode);
	LeaveCriticalSection(&m_cs);
}
//连接摄像头
void CDeviceTree::StartCamera(CString sStartCameraID)
{
	NodeInfo sCameraNode,sParentNode;
	SCameraInfo sCamera;
	sCamera.strCameraID = sStartCameraID;
	sCamera.nCount = 1;
	EnterCriticalSection(&m_cs);
	GetItemNodeInfo(sStartCameraID,sCameraNode);
	GetItemNodeInfo(sCameraNode.strParentNodeID,sParentNode);
	int i = 0;
	for (;i < sParentNode.m_vOpenCameraIDList.size(); i ++)
	{
		if (sStartCameraID == sParentNode.m_vOpenCameraIDList[i].strCameraID)
		{
			sParentNode.m_vOpenCameraIDList[i].nCount++;
			break;
		}
	}
	if (i == sParentNode.m_vOpenCameraIDList.size())
	{
		sParentNode.m_vOpenCameraIDList.push_back(sCamera);
		SetItemColor(sCameraNode.hTreeItem,m_SelCol);
	//	SetItemText(sCameraNode.hTreeItem,sCameraNode.sShowName);
	//	SetItemBold(sCameraNode.hTreeItem,TRUE);
	}
	UpdateTreeNode(sParentNode);
	LeaveCriticalSection(&m_cs);
}

void CDeviceTree::GetStopCamera(CString sNodeID,SCameraInfo & sStopCamera)
{
	NodeInfo sNode;
	EnterCriticalSection(&m_cs);
	GetItemNodeInfo(sNodeID,sNode);
	if(sNode.m_vOpenCameraIDList.size() >= sNode.nPageCount && sNode.nPageCount != 0)//允许一次打开的摄像头数已满，删除第一个打开的摄像头
	{
		sStopCamera = sNode.m_vOpenCameraIDList[0];
	}
	LeaveCriticalSection(&m_cs);
}

//获取双击节点或是翻页动作要操作的摄像头列表
void CDeviceTree::GetPageCamera(int nUserType,CString sNodeID,vector<CString> &vCameraID)
{
	NodeInfo node;
	GetItemNodeInfo(sNodeID,node);
	int nIndex = 0;
	int j = 0;//j为当前压入容器摄像头个数
	nIndex = node.nCameraIndex;//当前要打开的摄像头在节点中的前一个位置
	if (node.nPageCount == 0 || node.nPageCount >= node.m_vSubCameraIDList.size())//不限制分屏
	{
		vCameraID = node.m_vSubCameraIDList;
		return;
	}
	while(j < node.nPageCount)
	{
		
		NodeInfo sCameraNode;
		TreeSXT tempCamera;
		GetItemNodeInfo(node.m_vSubCameraIDList[nIndex],sCameraNode);
		GetItemSXTInfo(sCameraNode.hTreeItem,tempCamera);
		if (tempCamera.Active && AbleSeeCamera(nUserType,node.m_vSubCameraIDList[nIndex]))
		{
			vCameraID.push_back(node.m_vSubCameraIDList[nIndex]);
			j ++;
		}	
		nIndex ++;
		if (nIndex >= node.m_vSubCameraIDList.size())//最后一次打开节点下的剩余摄像头（有可能小于一次允许打开的最大值）
		{
			nIndex = 0;
			break;
		}
	}
	node.nCameraIndex = nIndex;
	UpdateTreeNode(node,nIndex);
}

BOOL CDeviceTree::FindChildItem(NodeInfo sParentNode)
{
	HTREEITEM hNext = GetChildItem(sParentNode.hTreeItem);
	if (hNext != NULL)
		return TRUE;
	return FALSE;
}

BOOL CDeviceTree::SelectNode(CString sNodeName,HTREEITEM startNode)
{
	int nCount = 0;
	if (sNodeName != m_sFindNodeName)
	{
		m_nCount = 0;
		m_sFindNodeName = sNodeName;
	}
	if (startNode != NULL)
	{
		m_nCount = 0;
		m_StartFindNode = startNode;
	}
	return FindNode(sNodeName,m_StartFindNode,nCount);
}

BOOL CDeviceTree::FindNode(CString sNodeName,HTREEITEM hNowTreeItem,int &nCount)
{
	HTREEITEM hCur,hNext;
	BOOL res = FALSE;
	while(hNowTreeItem != NULL)
	{
		CString tempStr;
		hCur = GetNextSiblingItem(hNowTreeItem);
		tempStr = GetItemText(hNowTreeItem);
		if (tempStr.Find(sNodeName) >= 0)
		{
			NodeInfo tempNode;
			GetItemNodeInfo(hNowTreeItem,tempNode);
			if (!tempNode.IsSXT)
			{
				nCount ++;
				if (m_nCount < nCount)
				{
					SelectItem(hNowTreeItem);
					m_nCount = nCount;
					return TRUE;
				}
			}
		}
		hNext = GetChildItem(hNowTreeItem);
		if (hNext != NULL)
		{
			res = FindNode(sNodeName,hNext,nCount);
			if (res)
				return TRUE;
		}
		hNowTreeItem = hCur;
	}
	
	return FALSE;
}


