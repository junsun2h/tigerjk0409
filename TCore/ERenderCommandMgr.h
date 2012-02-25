#pragma once


class ERenderCommandMgr : public IRenderCommandMgr
{
public:
	ERenderCommandMgr();
	~ERenderCommandMgr();

	friend unsigned int WINAPI  _RenderThreadProc( LPVOID lpParameter );

private:
	void								RT_SignalFlushFinshed();
	unsigned int						RT_RenderThreadProc();
	void								RT_ProcessCommand();

	bool								InitAsyncRenderThreadObjects() override;
	virtual void						AsyncRender(CCAMERA_DESC* pCameraDesc, IRenderingCallback* pRenderCallback) override;
	CCommandBuffer<eRENDER_COMMAND>*	GetFillCommandQueue() override	{	return &m_CommandQueue[m_FillBufferID]; }

	void								FlushAndWait() override;
	void								FlushWithoutWait() override;

	void								WaitUntilFlushFinished();
	
	bool								IsRenderThread();
	bool								IsMainThread();

private:	
	HANDLE							m_hRenderThread;

	UINT							m_RenderThreadID;
	UINT							m_MainThreadID;

	CCommandBuffer<eRENDER_COMMAND>	m_CommandQueue[NUM_RENDER_BUFFER];
	int								m_FillBufferID;
	int								m_ProcBufferID;

	BOOL							m_bDone;
	bool							m_bReadyToFlush;
};



