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
	LPVOID	m_pCalled;				//创建线程的使用者
	HANDLE	m_hThread;				//线程句柄
private:
	BOOL	m_bStopThreadFlag;	//停止线程运行	
	DWORD	m_dLiveTime;				//线程存在的时间戳
	int		m_nIndex;
	DWORD	m_dThreadID;			//线程ID
	HANDLE	m_hStopThreadEvent;		//通知线程停止的事件
	CCriticalSection	m_CS;
};
