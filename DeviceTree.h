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
	char JIeDian_id[50];// �ڵ�ID ,
	char JIeDian_name[50];// �ڵ����ƣ�
	char Parent_id[50];// ���ڵ�ID,
	long Active;// �ڵ�״̬1���ߣ�����
	long SXT_Count;// �¼�����ͷ��	
	long JieDian_Count;//�¼��ڵ���
	int	 nMaxOpenCameraCount;//���һ��ͬʱ�򿪵�����ͷ����Ϊȫ���򿪣�
};

struct	TreeSXT
{
	char SXT_id[50];			//����ͷID
	char SXT_Name[50];		//,����ͷ����
	char JieDian_id[50];		//�ڵ�ID
	char Server_ip[20];		//,ת����IP
	long Server_Port;		//,ת����˿�
	char YouXiaoQi[50];		//,����ʱ��
	bool atTerm;			//�Ƿ���
	long Active;			// ����ͷ״̬		1���ߣ�0����
	long acquisitionType;	// �豸����
	long YunTai_Flag;		// �Ƿ�����̨		1��ʾ�ǣ�0��ʾ����
	long AudioSwitch;		//��Ƶ����			1������0����
	int		ZhiBo_Flag;		//�Ƿ�ֱ������ͷ	1��ʾ�ǣ�0��ʾ����
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
	int nCount;//��ǰ����ͷ�򿪵�����
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
	int nPageCount;//ÿ���ڸýڵ�������nPageCount������ͷ����
	vector<CString> m_vSubCameraIDList;//������ͷID����
	vector<SCameraInfo> m_vOpenCameraIDList;//�Ѿ��򿪵�����ͷID����
	int nCameraIndex;//��ǰҪ�򿪵�����ͷ������
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
	//�����������ػ�ڵ��������ǿ�������ͷ��
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
	void CheckSubItem(HTREEITEM hNowTreeItem, BOOL bCheck);	// ��ѡ���ڵ�ʱ�������ӽڵ�Ҳ��ѡ
	void GetChooseNodeList();								//��ȡ��ǰ���й�ѡ�Ľڵ���Ϣ
	
	void TreeToStr(CString &str);//�ѹ�ѡ������ת���ַ���
	void StrToTree(CString str);//���ַ���ת������(��������)
	void ClearSubTree();	//��ʼ������Ϊ��
	void InitTreeNodeNoCheck();//��ʼ�����нڵ�Ĺ�ѡ״̬Ϊûѡ��״̬
	void ShowSubTreeNodeCheck();//��ʾ���������ϣ��ù�ѡ���״̬��ʾ
	void SetCheckState(HTREEITEM hNowTreeItem, BOOL bCheck);//����ָ���ڵ�Ĺ�ѡ��״̬
	BOOL GetCheckNodeTree(HTREEITEM hNowTreeItem);	//�ӵ�ǰ���л�ȡ��ѡ������
	void GetNodeListForParent(vector<NodeInfo> &vNodeTree,CString parentNodeID);	// ��ȡ���ڵ���һ�������нڵ㼯��
	void NodeIDListToStr(CString &str,vector<NodeInfo> vNodeTree);					//��װҪ�����������ڵ���Ϣ�Ľڵ�ID�б��ַ���
	void GetSubTree(vector<NodeInfo> &vSubTree);
	void NodeListToStr(vector<NodeInfo> vSubTree,CString &sSqlStr);//��װ�ɷ������ݿ���Ҫ���ַ���
	int  GetSubTreeSize();
	
	BOOL CopyTree(CDeviceTree * treeFrom,HTREEITEM itemFrom=TVI_ROOT,HTREEITEM itemTo=TVI_ROOT);//������
	BOOL CopyData_tree2tree(CDeviceTree * treeFrom,HTREEITEM itemFrom=TVI_ROOT,HTREEITEM itemTo=TVI_ROOT);

	void SetParentCWnd(LPVOID pParent)
	{
		m_pParent = pParent;
	}
	BOOL FindChildItem(NodeInfo sParentNode);
	BOOL SelectNode(CString sNodeName,HTREEITEM startNode = NULL);
	BOOL FindNode(CString sNodeName,HTREEITEM hNowTreeItem,int &nCount);
	void UpdateTreeNode(NodeInfo nNodeInfo,int index = -1);//�������ڵ���Ϣ����Ҫ��������ͷ�򿪵���Ϣ��
	BOOL AbleSeeCamera(int nUserType,CString sCameraID);
	void ChangeOpenCameraIndex(CString sNodeID,CString sCameraID);
	void StopCamera(CString sCameraID);//�رտؼ�
	void StartCamera(CString sStartCameraID);//�����ؼ�
	void GetStopCamera(CString sNodeID,SCameraInfo & sStopCamera);//��ȡҪ�رյ�����ͷ�ɣ�
	void GetPageCamera(int nUserType,CString sNodeID,vector<CString> &vCameraID);//˫���ڵ���Ƿ�ҳ������ͷ�б�һ�δ򿪼�������ͷ��
private:
	CRITICAL_SECTION m_cs;
	CImageList	m_TreeImage;
	map<HTREEITEM, NodeInfo>	m_mapTree;
	map<HTREEITEM, TreeSXT>	m_mapSXT;
	vector<NodeInfo> m_mSubTree;	//����
	CMap< void*, void*, Color_Font, Color_Font& > m_mapColorFont ;
	LPVOID m_pParent;
	COLORREF m_SelCol;//ѡ����ɫ
	CString m_sFindNodeName;//�ϴβ鵽�Ľڵ���
	int		m_nCount;//��ѯ�ĵڼ���
	HTREEITEM m_StartFindNode;
public:
	COLORREF m_InitCol;//��ʼ����ɫ
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};
