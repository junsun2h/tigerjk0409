#pragma once

#include "IAsyncLoader.h"
#include "CGrowableArray.h"

class EAssetMgr;

class EAsyncLoader : public IAsyncLoader
{
private:
	EAssetMgr*	m_pAssetMgr;
	BOOL	m_bDone;
	BOOL	m_bProcessThreadDone;
	BOOL	m_bIOThreadDone;
	UINT	m_NumOustandingResources;

	HANDLE	m_hIOQueueSemaphore;
	HANDLE	m_hProcessQueueSemaphore;
	HANDLE	m_hIOThread;
	UINT	m_NumProcessingThreads;
	HANDLE* m_phProcessThreads;

	CGrowableArray <RESOURCE_REQUEST> m_IOQueue;
	CGrowableArray <RESOURCE_REQUEST> m_ProcessQueue;
	CGrowableArray <RESOURCE_REQUEST> m_MainThreadQueue;
	CRITICAL_SECTION	m_csIOQueue;
	CRITICAL_SECTION	m_csProcessQueue;
	CRITICAL_SECTION	m_csMainThreadQueue;

private:
	unsigned int                IOT_FileIOThreadProc();
	unsigned int                PT_ProcessingThreadProc();
	bool                        MT_InitAsyncLoadingThreadObjects( UINT NumProcessingThreads );
	void                        MT_DestroyAsyncLoadingThreadObjects();

public:
	friend unsigned int WINAPI  _FileIOThreadProc( LPVOID lpParameter );
	friend unsigned int WINAPI  _ProcessingThreadProc( LPVOID lpParameter );

	EAsyncLoader( UINT NumProcessingThreads );
	virtual ~EAsyncLoader();

	virtual void	MT_AddWorkItem( RESOURCE_REQUEST resourceRequest) override;
	virtual void    MT_WaitForAllItems(IAssetMgr* pAssetMgr) override;
	virtual void    MT_CompleteWork( UINT CurrentNumResourcesToService, IAssetMgr* pAssetMgr ) override;
};
