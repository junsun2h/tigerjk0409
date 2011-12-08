#pragma once

#include "CThread.h"

class CMultiThreadTLoader;


class CMultiThreadLoadUnit
{
	friend CMultiThreadTLoader;
public:
	CMultiThreadLoadUnit() 
		: m_bExpired(false)
		, m_nState(LOAD_NONE)
		, m_pMultiThreadWorker(NULL)
	{
	}

	enum LOAD_STATE
	{
		LOAD_NONE,
		LOAD_FINISHED
	};

	enum WORKING_SPOT
	{
		MAIN_THREAD,
		WORK_THREAD
	};

	LOAD_STATE				GetState()						{ return m_nState; }
	virtual const char*		GetName()						{ return "None"; }

	bool					IsBackgroundWork()				{ return (m_pMultiThreadWorker != NULL); }
	bool					IsFinished()					{ return (m_nState == LOAD_FINISHED) ; }
	bool					IsExpired()						{ return m_bExpired; }
	bool					IsLoadingCompleted()			{ return IsFinished() || IsExpired();}
	
	void					Invalidate()					{ Lock(); m_bExpired = true; Unlock();}

	//////////////////////////////////////////////////////////////////////////
	// MWT_ = Call from Main & Work Tread
	// MT_ = Call from Main Thread, It shouldn't call from Work Thread	
	// WT_ = Call from Work Thread, It shouldn't call from Main Thread

	virtual void			MWT_OnWork(WORKING_SPOT workPlace) = 0;
	virtual bool			MT_OnUpdateCompleteWork() = 0;
	void					MT_FinishWork()					{ Lock(); m_nState = LOAD_FINISHED; Unlock(); }
	void					WT_WorkBackground()				{ MWT_OnWork(WORK_THREAD);}
	void					MT_WorkForeground()	// to load without Loader.
	{
		while(true)
		{
			MWT_OnWork(MAIN_THREAD);	
			if( MT_OnUpdateCompleteWork() )
			{
				MT_FinishWork();
				break;
			}
		}
	}

protected:
	CMultiThreadTLoader*		m_pMultiThreadWorker;

	void	Lock(void)		{	m_Lock.Lock();		}
	void	Unlock(void)	{	m_Lock.Unlock();	}

	CCriticalSectionSpinLock	m_Lock;
	bool						m_bExpired;
	LOAD_STATE					m_nState;
};
