#include <process.h>
#include "MultiThreadLoader.h"

unsigned int WINAPI CMultiThreadTLoader::LoadThreadProc(void* _pWorker)
{
	CMultiThreadTLoader* pWorker = (CMultiThreadTLoader*)_pWorker;

	CMultiThreadLoadUnit* pUnit;
	while(pWorker->m_JobToDo.PopFront(pUnit,INFINITE))
	{
		if( NULL == pUnit )	
			break;

		pUnit->Lock();
		if (!pUnit->IsExpired())
		{
			pUnit->MWT_OnWork( CMultiThreadLoadUnit::WORK_THREAD );
		}
		pUnit->Unlock();

		pWorker->m_JobDone.PushBack(pUnit);
	}

	int exitValue = 0;
	_endthreadex(exitValue);
	return exitValue;
}

CMultiThreadTLoader::CMultiThreadTLoader() 
	: m_bInited(false)
{
}

void CMultiThreadTLoader::StartUp()
{
	assert( IsInited() == false );

	unsigned int dwThreadIdDumy(0);
	m_hThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, LoadThreadProc, (void*)this, 0, &dwThreadIdDumy));
	assert(m_hThread);

	m_bInited = true;
}

void CMultiThreadTLoader::ShutDown()
{
	assert(IsInited());

	WaitForSingleObject(m_hThread,INFINITE);	
	ClearQueues();
	CloseHandle(m_hThread);
	m_hThread = NULL;
	m_bInited = false;
}

void CMultiThreadTLoader::AddToReadyQueue( CMultiThreadLoadUnit* pUnit )
{
	assert(IsInited());

	if ( pUnit == NULL ) 
		return;

	m_JobToDo.PushBack(pUnit);
}

void CMultiThreadTLoader::ClearQueues()
{
	assert(IsInited());

	m_JobToDo.Clear();
	m_JobDone.Clear();
	ClearWorkUnitDeleteQueue();
}

void CMultiThreadTLoader::Update()
{
	assert(IsInited());

	CMultiThreadLoadUnit* pUnit;
	while (m_JobDone.TryPopFront(pUnit))
	{
		if ( pUnit->IsExpired() == false )
		{
			if( pUnit->MT_OnUpdateCompleteWork() )
			{
				pUnit->MT_FinishWork();
			}
		}
		else
		{
			pUnit->MT_FinishWork();
		}
	}

	LOAD_JOB_LIST::iterator itor = m_JobToDelete.begin();
	while(itor != m_JobToDelete.end())
	{
		CMultiThreadLoadUnit* pUnit = *itor;
		if( pUnit->IsFinished() )
		{
			delete pUnit;
			itor = m_JobToDelete.erase(itor);
			continue;
		}
		++itor;
	}
}

int CMultiThreadTLoader::GetRemainedJob()
{
	return (int)m_JobToDo.Size() + m_JobDone.Size();
}

void CMultiThreadTLoader::AsyncDelete( CMultiThreadLoadUnit* pUnit )
{
	assert(IsInited());

	m_JobToDelete.push_back(pUnit);
}

void CMultiThreadTLoader::ClearWorkUnitDeleteQueue()
{
	assert(IsInited());
	
	LOAD_JOB_LIST::iterator itor = m_JobToDelete.begin();
	for( ; itor != m_JobToDelete.end() ; ++itor )
	{
		CMultiThreadLoadUnit* pUnit = *itor;
		delete pUnit;
	}

	m_JobToDelete.clear();
}