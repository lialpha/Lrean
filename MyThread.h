#pragma once
#include "afxmt.h" // CCritical section.. 
#define CheckThreadLiveTime		30 * 1000

typedef UINT ThreadFun(LPVOID pParam);
class CMyThread
{
public:
	CMyThread(void);
	~CMyThread(void);

	void SetThreadIndex(int nIndex = 0);
	
	int GetThreadIndex();
	int GetThreadID();
	BOOL CheckThreadLive(DWORD dCheckTime);
	void UpdateThreadLiveTime();
	BOOL CreateMyThread(ThreadFun *pThreadFun,LPVOID pCalled);
	BOOL WaitTimeOut(DWORD dWaitTime);
	void StopMyThread();
	
public:
	LPVOID	m_pCalled;				//�����̵߳�ʹ����
	HANDLE	m_hThread;				//�߳̾��
private:
	BOOL	m_bStopThreadFlag;	//ֹͣ�߳�����	
	DWORD	m_dLiveTime;				//�̴߳��ڵ�ʱ���
	int		m_nIndex;
	DWORD	m_dThreadID;			//�߳�ID
	HANDLE	m_hStopThreadEvent;		//֪ͨ�߳�ֹͣ���¼�
	CCriticalSection	m_CS;
};
