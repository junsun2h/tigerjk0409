#pragma once

#include "IAsyncLoader.h"
#include "CGrowableArray.h"

class EAssetMgr;

#define MAX_DATA_PROC_THREAD	3

class EAsyncLoader : public IAsyncLoader
{
private:
	EAssetMgr*			m_pAssetMgr;
	BOOL				m_bDone;
	BOOL				m_bProcessThreadDone;
	BOOL				m_bIOThreadDone;
	UINT				m_NumOustandingResources;

	HANDLE				m_hIOQueueSemaphore;
	HANDLE				m_hProcessQueueSemaphore;
	HANDLE				m_hIOThread;
	HANDLE				m_phProcessThreads[MAX_DATA_PROC_THREAD];
	
	static UINT	m_IOThreadID;
	static UINT	m_ProcessThreadID[MAX_DATA_PROC_THREAD];
	static UINT	m_NumProcessingThreads;

	CGrowableArray <RESOURCE_REQUEST> m_IOQueue;
	CGrowableArray <RESOURCE_REQUEST> m_ProcessQueue;
	CGrowableArray <RESOURCE_REQUEST> m_MainThreadQueue;
	CRITICAL_SECTION	m_csIOQueue;
	CRITICAL_SECTION	m_csProcessQueue;
	CRITICAL_SECTION	m_csMainThreadQueue;

private:
	unsigned int                IOT_FileIOThreadProc();
	unsigned int                PT_ProcessingThreadProc();
	void                        DestroyAsyncLoadingThreadObjects();

public:
	EAsyncLoader();
	virtual ~EAsyncLoader();


	static	bool				IsIOThread();
	static	bool				IsDataProcThread();

	bool                        Init( UINT NumProcessingThreads, EAssetMgr* pAssetMgr );
	
	friend unsigned int WINAPI  _FileIOThreadProc( LPVOID lpParameter );
	friend unsigned int WINAPI  _ProcessingThreadProc( LPVOID lpParameter );
	

	virtual void	AddWorkItem( RESOURCE_REQUEST resourceRequest) override;
	virtual void    WaitForAllItems(IAssetMgr* pAssetMgr) override;
	virtual void    CompleteWork( UINT CurrentNumResourcesToService, IAssetMgr* pAssetMgr ) override;
};
