#pragma once

#include "MultiThreadLoadUnit.h"
#include "CThreadSafeQueue.h"
#include <list>



class CMultiThreadTLoader
{
public:
	CMultiThreadTLoader();
	~CMultiThreadTLoader();

	static unsigned int WINAPI LoadThreadProc(void* _pWorker);
	
	void			StartUp();
	void			ShutDown();
	bool			IsInited() { return m_bInited; }

	void			AddToReadyQueue( CMultiThreadLoadUnit* pWorkUnit );

	void			Update();
	void			ClearQueues();
	void			ClearWorkUnitDeleteQueue();
	int				GetRemainedJob();
	void			AsyncDelete(CMultiThreadLoadUnit* pBackgroundWorkUnit);
	
private:
	HANDLE			m_hThread;
	bool			m_bInited;

	typedef CThreadSafeQueue<CMultiThreadLoadUnit*>	LOAD_JOB_QUEUE;
	typedef std::list<CMultiThreadLoadUnit*>		LOAD_JOB_LIST;

	LOAD_JOB_QUEUE	m_JobToDo;
	LOAD_JOB_QUEUE	m_JobDone;
	LOAD_JOB_LIST	m_JobToDelete;
};