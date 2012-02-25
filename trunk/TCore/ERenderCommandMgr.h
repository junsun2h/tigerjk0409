#pragma once




class ERenderCommandMgr : public IRenderCommandMgr
{
	CGrowableArray <eRENDER_COMMAND> m_Jobs;
	CRITICAL_SECTION			m_csRenderQueue;
	HANDLE						m_hRenderThread;
	HANDLE						m_hRenderQueueSemaphore;

	UINT						m_RenderThreadID;
	UINT						m_MainThreadID;

	CGrowableArray<byte>		m_CommandQueue[NUM_RENDER_BUFFER];
	int							m_FillBufferID;
	int							m_ProcBufferID;

	BOOL						m_bDone;
	bool						m_bReadyToFlush;

	IRDevice*					m_pRDevice;

public:
	ERenderCommandMgr();
	~ERenderCommandMgr();

	bool                        InitAsyncRenderThreadObjects();
	void						WaitUntilFlushFinished();
private:
	void						RT_SignalFlushFinshed();

private:
	byte*						AddCommand(eRENDER_COMMAND cmd, CRenderCommandBase* pCommand) = 0;
	unsigned int                RT_RenderThreadProc();
	void		                RT_ProcessCommand();

	friend unsigned int WINAPI  _RenderThreadProc( LPVOID lpParameter );

	bool						IsRenderThread();
	bool						IsMainThread();


	void						FlushAndWait();
	void						FlushWithoutWait();

private:	
	void						AddDWORD(byte*& ptr, uint32 nVal);
};



