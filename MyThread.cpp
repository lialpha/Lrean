#include "StdAfx.h"
#include "MyThread.h"

CMyThread::CMyThread(void)
{
	m_hThread = NULL;
	m_hStopThreadEvent = NULL;
	m_dThreadID = 0;
	m_pCalled = NULL;
	m_nIndex = -1;
	m_bStopThreadFlag = FALSE;
}

CMyThread::~CMyThread(void)
{
	StopMyThread();
}

void CMyThread::SetThreadIndex(int nIndex)
{
	m_nIndex = nIndex;
}

int  CMyThread::GetThreadIndex()
{
	return m_nIndex;
}

int CMyThread::GetThreadID()
{
	return m_dThreadID;
}

BOOL CMyThread::CheckThreadLive(DWORD dCheckTime)
{
	BOOL bResult = FALSE;
	m_CS.Lock();
	if (labs(GetTickCount() - m_dLiveTime) > dCheckTime)
		bResult = FALSE;
	else
		bResult = TRUE;
	m_CS.Unlock();
	return bResult;
}

void CMyThread::UpdateThreadLiveTime()
{
	m_dLiveTime = GetTickCount();
}

BOOL CMyThread::CreateMyThread(ThreadFun *pThreadFun,LPVOID pCalled)
{
	m_CS.Lock();
	m_hStopThreadEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	if (m_hStopThreadEvent == NULL)
	{
		m_CS.Unlock();
		return FALSE;
	}
	m_pCalled = pCalled;
	m_dLiveTime = GetTickCount();
	m_hThread = CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(pThreadFun),this,0,&m_dThreadID);
	if (m_hThread == NULL)
	{
		CloseHandle(m_hStopThreadEvent);
		m_hStopThreadEvent = NULL;
		m_CS.Unlock();
		return FALSE;
	}
	m_CS.Unlock();
	return TRUE;
}

BOOL CMyThread::WaitTimeOut(DWORD dWaitTime)
{
	if(dWaitTime <= 0)
		return FALSE;

	if (WaitForSingleObject(m_hStopThreadEvent,dWaitTime) == WAIT_TIMEOUT)
	{
		UpdateThreadLiveTime();
		return TRUE;
	}
	else
		return FALSE;
}

void CMyThread::StopMyThread()
{
	m_CS.Lock();
	if (m_hThread != NULL)
	{
		SetEvent(m_hStopThreadEvent);
		WaitForSingleObject(m_hThread,INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
		CloseHandle(m_hStopThreadEvent);
		m_hStopThreadEvent = NULL;
	}
	m_CS.Unlock();
}
