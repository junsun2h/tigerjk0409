#pragma once

#define MAX_DATA_PROC_THREAD	3

class ELoader : public ILoader
{
public:
	ELoader();
	virtual ~ELoader();

	bool                        Init( UINT NumProcessingThreads ) override;
	bool						IsIOThread() override;
	bool						IsDataProcThread() override;

public:
	virtual CResourceBase*		LoadForward(char* fileName, char* name, eRESOURCE_FILE_TYPE type) override;
	virtual void				LoadBackword(char* fileName, char* name, eRESOURCE_FILE_TYPE type) override;
	virtual void				WaitForAllItems() override;
	virtual void				CompleteWork( UINT CurrentNumResourcesToService ) override;

private:


	friend unsigned int WINAPI  _FileIOThreadProc( LPVOID lpParameter );
	friend unsigned int WINAPI  _ProcessingThreadProc( LPVOID lpParameter );

	unsigned int                IOT_FileIOThreadProc();
	unsigned int                PT_ProcessingThreadProc();
	void                        DestroyAsyncLoadingThreadObjects();
	
private:
	BOOL						m_bDone;
	BOOL						m_bProcessThreadDone;
	BOOL						m_bIOThreadDone;
	UINT						m_NumOustandingResources;

	HANDLE						m_hIOQueueSemaphore;
	HANDLE						m_hProcessQueueSemaphore;
	HANDLE						m_hIOThread;
	HANDLE						m_phProcessThreads[MAX_DATA_PROC_THREAD];

	static UINT					m_IOThreadID;
	static UINT					m_ProcessThreadID[MAX_DATA_PROC_THREAD];
	static UINT					m_NumProcessingThreads;

	CGrowableArray <RESOURCE_REQUEST> m_IOQueue;
	CGrowableArray <RESOURCE_REQUEST> m_ProcessQueue;
	CGrowableArray <RESOURCE_REQUEST> m_MainThreadQueue;

	CRITICAL_SECTION			m_csIOQueue;
	CRITICAL_SECTION			m_csProcessQueue;
	CRITICAL_SECTION			m_csMainThreadQueue;
};
