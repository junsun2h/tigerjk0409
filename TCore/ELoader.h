#pragma once

#define MAX_DATA_PROC_THREAD	3

class ELoader : public ILoader
{
public:
	ELoader();
	virtual ~ELoader();
	bool						IsIOThread() override;
	bool						IsDataProcThread() override;

	bool                        Init( UINT NumProcessingThreads ) override;
	CResourceBase*				Load(char* name, eRESOURCE_FILE_TYPE type, bool bWait) override;
	CResourceBase*				Load(const char* fullpath, char* name, eRESOURCE_FILE_TYPE type, bool bForward) override;

	void						WaitForAllItems() override;
	void						CompleteWork( UINT completeLimit ) override;

private:
	friend unsigned int WINAPI  _FileIOThreadProc( LPVOID lpParameter );
	friend unsigned int WINAPI  _ProcessingThreadProc( LPVOID lpParameter );

	unsigned int                IOT_FileIOThreadProc();
	unsigned int                PT_ProcessingThreadProc();
	void                        DestroyAsyncLoadingThreadObjects();
	
private:
	char						m_Path[MAX_PATH];

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

	CRITICAL_SECTION			m_csIOQueue;
	CRITICAL_SECTION			m_csProcessQueue;
	CRITICAL_SECTION			m_csMainThreadQueue;
};
