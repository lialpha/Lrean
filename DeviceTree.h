#pragma once
#include "afxcmn.h"
#include <map>
#include <vector>
using namespace std;

#define MSG_NMCHECKNODE			WM_USER + 100
#define MSG_NMCLICK				WM_USER + 101
#define MSG_NODE_DBLCLICK		WM_USER + 102

enum{
	SUPER_USER,
	MAINUSER,
	SUBUSER
};

struct	TreeJiedian
{
	char JIeDian_id[50];// 节点ID ,
	char JIeDian_name[50];// 节点名称，
	char Parent_id[50];// 父节点ID,
	long Active;// 节点状态1在线，离线
	long SXT_Count;// 下级摄像头数	
	long JieDian_Count;//下级节点数
	int	 nMaxOpenCameraCount;//最多一次同时打开的摄像头数（为全部打开）
};

struct	TreeSXT
{
	char SXT_id[50];			//摄像头ID
	char SXT_Name[50];		//,摄像头名称
	char JieDian_id[50];		//节点ID
	char Server_ip[20];		//,转发层IP
	long Server_Port;		//,转发层端口
	char YouXiaoQi[50];		//,到期时间
	bool atTerm;			//是否到期
	long Active;			// 摄像头状态		1在线，0离线
	long acquisitionType;	// 设备类型
	long YunTai_Flag;		// 是否是云台		1表示是，0表示不是
	long AudioSwitch;		//音频开关			1：开，0：关
	int		ZhiBo_Flag;		//是否直播摄像头	1表示是，0表示不是
	TreeSXT()
	{
		memset(SXT_id,0,50);
		memset(JieDian_id,0,50);
		memset(SXT_Name,0,50);
		memset(Server_ip,0,20);
	}
};


typedef vector<TreeJiedian> TREEJIEDIAN;
typedef vector<TreeJiedian>::iterator TREEJIEDIANIt;
typedef vector<TreeSXT> TREESXT;
typedef vector<TreeSXT>::iterator TREESXTIt;

struct SCameraInfo
{
	CString strCameraID;
	int nCount;//当前摄像头打开的屏数
	SCameraInfo()
	{
		strCameraID.Empty();
		nCount = 0;
	}
};

struct	NodeInfo
{
	CString sNodeName;
	CString sShowName;
	BOOL IsSXT;
	CString sNodeID;
	HTREEITEM hTreeItem;
	CString strParentNodeID;
	int nSubNodeCount;
	int nSubCameraCount;
	BOOL Check;
	int nPageCount;//每次在该节点下最多打开nPageCount个摄像头连接
	vector<CString> m_vSubCameraIDList;//子摄像头ID集合
	vector<SCameraInfo> m_vOpenCameraIDList;//已经打开的摄像头ID集合
	int nCameraIndex;//当前要打开的摄像头的索引
	void CreateNode(NodeInfo *pNode)
	{
		sNodeName = pNode->sNodeName;
		sShowName = pNode->sShowName;
		IsSXT = pNode->IsSXT;
		sNodeID = pNode->sNodeID;
		hTreeItem = pNode->hTreeItem;
		strParentNodeID = pNode->strParentNodeID;
		nSubCameraCount = pNode->nSubCameraCount;
		nSubNodeCount = pNode->nSubNodeCount;
		Check = pNode->Check;
		nPageCount = pNode->nPageCount;
		m_vSubCameraIDList = pNode->m_vSubCameraIDList;
		m_vOpenCameraIDList = pNode->m_vOpenCameraIDList;
		nCameraIndex = pNode->nCameraIndex;
	}
	void Init()
	{
		sNodeName = "";
		sShowName.Empty();
		IsSXT = FALSE;
		sNodeID = "";
		hTreeItem = NULL;
		strParentNodeID = "";
		nSubNodeCount = 0;
		nSubCameraCount = 0;
		Check = FALSE;
		nPageCount = 0;
		nCameraIndex = 0;
	}
	int FindCamera(CString sCameraID)
	{
		for (int i = 0; i < m_vOpenCameraIDList.size(); i++)
		{
			if (m_vOpenCameraIDList[i].strCameraID == sCameraID)
			{
				return i;
			}
		}
		return -1;
	}
};

struct Color_Font
{
	COLORREF color;
	LOGFONT  logfont;
};
class CDeviceTree :
	public CTreeCtrl
{
public:
	CDeviceTree(void);
	~CDeviceTree(void);
	void init();
	void InitTopTree(CString sUserName,CString sClubID,TREEJIEDIAN TreeList);
	//生成树，返回活动节点数或者是可用摄像头数
	int  CreateNodeTree(NodeInfo nNodeInfo,TREEJIEDIAN TreeList,TREESXT TreeSXT);
	
	void DeleteAll();
	void UpdateCheck(HTREEITEM hItem,BOOL check);
	HTREEITEM  NewTopZoneToTree(CString  sNodeName,CString  sNodeID,BOOL bHasSub);
	HTREEITEM  AlterItemNodeInfo(CString nodeId,CString parentNodeId,CString strNewNodeName,int nPageCount,HTREEITEM hItem);
	int        AddChildZoneToTree(CString strParentNodeID,CString  sNodeName,CString  sNodeID,int nSubNodeCount,int nMaxOpenCameraCount,
		int  nSubCameraCount,BOOL IsSXT,BOOL atTerm,BOOL bActive,HTREEITEM hItem);
	void  AddSXTToTree(HTREEITEM hItem,TreeSXT  &sTreeSXT)
	{
		if(NULL==hItem)
			return;
		if(m_mapSXT.find(hItem) != m_mapSXT.end())
			return;
		m_mapSXT[hItem] = sTreeSXT;
	}
	void AddNodeCount(CString nodeId);
	void SubtractNodeCount(CString nodeId);
	void SubtractCameraCount(CString nodeId);
	void DeleteNodeForMapTree(CString nodeID);
	void DeleteNodeForMapSXT(CString nodeID);
	void DeleteNodeForTree(CString nodeID);
	void DeleteCameraNodeForTree(CString nodeId,CString sCameraId);
	void DeleteCameraForMapSXT(CString sCameraId);

	BOOL GetItemNodeInfo(HTREEITEM hItem,NodeInfo &sNodeInfo);
	BOOL GetItemNodeInfo(CString nodeId,NodeInfo &sNodeInfo);
	BOOL GetItemSXTInfo(HTREEITEM hItem,TreeSXT &sTreeSXT);
	int	 GetNodeSubItemSXT(CString sNodeID,TREESXT &vTreeSXT);
	BOOL GetItemSXTInfo(CString sCameraID,NodeInfo &sNodeInfo);
	BOOL GetItemSXTInfo(CString sCameraID,TreeSXT &sTreeSXT);
	void SetItemFont(HTREEITEM hItem, LOGFONT& logfont);
	void SetItemBold(HTREEITEM hItem, BOOL bBold);
	void SetItemColor(HTREEITEM hItem, COLORREF color);
	BOOL GetItemFont(HTREEITEM hItem, LOGFONT * plogfont);
	BOOL GetItemBold(HTREEITEM hItem);
	COLORREF GetItemColor(HTREEITEM hItem);
	void MakePaint();
	void CheckSubItem(HTREEITEM hNowTreeItem, BOOL bCheck);	// 勾选父节点时，所有子节点也勾选
	void GetChooseNodeList();								//获取当前所有勾选的节点信息
	
	void TreeToStr(CString &str);//把勾选的子树转成字符串
	void StrToTree(CString str);//从字符串转成子树(生成子树)
	void ClearSubTree();	//初始化子树为空
	void InitTreeNodeNoCheck();//初始化所有节点的勾选状态为没选中状态
	void ShowSubTreeNodeCheck();//显示子树到树上，用勾选框的状态显示
	void SetCheckState(HTREEITEM hNowTreeItem, BOOL bCheck);//设置指定节点的勾选框状态
	BOOL GetCheckNodeTree(HTREEITEM hNowTreeItem);	//从当前树中获取勾选的子树
	void GetNodeListForParent(vector<NodeInfo> &vNodeTree,CString parentNodeID);	// 获取父节点下一级的所有节点集合
	void NodeIDListToStr(CString &str,vector<NodeInfo> vNodeTree);					//组装要向服务器请求节点信息的节点ID列表字符串
	void GetSubTree(vector<NodeInfo> &vSubTree);
	void NodeListToStr(vector<NodeInfo> vSubTree,CString &sSqlStr);//组装成访问数据库需要的字符串
	int  GetSubTreeSize();
	
	BOOL CopyTree(CDeviceTree * treeFrom,HTREEITEM itemFrom=TVI_ROOT,HTREEITEM itemTo=TVI_ROOT);//拷贝树
	BOOL CopyData_tree2tree(CDeviceTree * treeFrom,HTREEITEM itemFrom=TVI_ROOT,HTREEITEM itemTo=TVI_ROOT);

	void SetParentCWnd(LPVOID pParent)
	{
		m_pParent = pParent;
	}
	BOOL FindChildItem(NodeInfo sParentNode);
	BOOL SelectNode(CString sNodeName,HTREEITEM startNode = NULL);
	BOOL FindNode(CString sNodeName,HTREEITEM hNowTreeItem,int &nCount);
	void UpdateTreeNode(NodeInfo nNodeInfo,int index = -1);//更新树节点信息（主要是子摄像头打开的信息）
	BOOL AbleSeeCamera(int nUserType,CString sCameraID);
	void ChangeOpenCameraIndex(CString sNodeID,CString sCameraID);
	void StopCamera(CString sCameraID);//关闭控件
	void StartCamera(CString sStartCameraID);//开启控件
	void GetStopCamera(CString sNodeID,SCameraInfo & sStopCamera);//获取要关闭的摄像头Ｉｄ
	void GetPageCamera(int nUserType,CString sNodeID,vector<CString> &vCameraID);//双击节点或是翻页打开摄像头列表（一次打开几个摄像头）
private:
	CRITICAL_SECTION m_cs;
	CImageList	m_TreeImage;
	map<HTREEITEM, NodeInfo>	m_mapTree;
	map<HTREEITEM, TreeSXT>	m_mapSXT;
	vector<NodeInfo> m_mSubTree;	//子树
	CMap< void*, void*, Color_Font, Color_Font& > m_mapColorFont ;
	LPVOID m_pParent;
	COLORREF m_SelCol;//选中颜色
	CString m_sFindNodeName;//上次查到的节点名
	int		m_nCount;//查询的第几次
	HTREEITEM m_StartFindNode;
public:
	COLORREF m_InitCol;//初始的颜色
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};
