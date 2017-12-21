#pragma once
#define CAPDATAPB			0			//�Ӳɼ��˵����ݻط�
#define SEVDATAPB			1			//�ӷ����������ݻط�
// CVideoPlay �Ի���

//��ǰ�Ļط�״̬�ص�
enum StatusType
{
	PLAY_ST = WM_USER + 100,//��ʼ״����ʾ����������1Ϊ��ʼ��0Ϊ������
	PAUSE_ST,				//��ͣ״����ʾ����������1Ϊ��ͣ��0Ϊ��ʼ��
	CHANGEAVI_ST,			//����AVI״̬��ʾ����������1Ϊ��ʼ��0Ϊ������
	CHANGESPEED_ST,			//�ı䲥���ٶȣ���������1Ϊ���٣�+2Ϊ2�����٣�-2Ϊ�������٣���2��4��8��-2��-4��

	SETSLDMAXVALUE_ST,		//���ý��������ֵ���ļ�������һ���ļ��¼��һ����,�ɼ��˵�¼��ģʽ���ã�
	SETSLIDERPOS_ST,		//���ý�����λ��
	SETTIMES_ST,			//���õ�ǰ����ʱ��

	CAPTURE_ST,				//��ͼ����״̬
	CLOSE_ST,				//�رս���

};

// PlayBack �Ի���

class PlayBack : public CDialog
{
	DECLARE_DYNAMIC(PlayBack)

public:
	PlayBack(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PlayBack();

// �Ի�������
	enum { IDD = IDD_DIALOG_PlayBack };

public:

	LPVOID  m_pDlg;
	HANDLE	m_hWorkThread;
	DWORD	m_dWordThreadID;
	LPVOID	m_pParentWnd;
	int		m_nPanelIndex;
	CStatic	m_ShowViewWnd;
	CString	m_sDir;							//���AVI�ļ��Լ�ץͼ��·��

	int		m_nType;						//�ط�ģʽ
	BOOL	m_bPause;						//��ͣ�벥��
	BOOL	m_bStart;						//��ʼ�����
	BOOL	m_bStartSaveAVI;				//AVI�ļ�����Ŀ���

	CTime	m_nStartDay;					//��ʼ����
	CTime	m_nStartTime;					//��ʼʱ��
	CTime	m_nStopDay;						//��������
	CTime	m_nStopTime;					//����ʱ��
	DATE	m_dNewTime;						//�϶��������²���ʱ��
	int		m_nPos;							//�϶���������λ��
	DATE	m_dBeginTime;					//��ʼʱ��
	DATE	m_dEndTime;						//����ʱ��

	CTimeSpan m_tSpan;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CString GetDirectoryPath();
public:
	afx_msg void OnBnClickedBtPlay();
	afx_msg void OnBnClickedBtStop();
	afx_msg void OnBnClickedBtFsbw();
	afx_msg void OnBnClickedBtFsfw();
	afx_msg void OnBnClickedBtCapture();
	afx_msg void OnBnClickedBtSaveavi();
	afx_msg void OnBnClickedBtSetpath();
	afx_msg void OnBnClickedBtClear();
	virtual BOOL OnInitDialog();
};
