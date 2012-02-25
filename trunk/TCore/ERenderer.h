#pragma once


class ERenderer : public IEngineRenderer
{
public:
	ERenderer();
	~ERenderer();

	friend unsigned int WINAPI  _RenderThreadProc( LPVOID lpParameter );

	//////////////////////////////////////////////////////////////////////////
	//	IRenderer
private:	
	void			RenderLine(CVertexPC* pVertex,int size) override;
	void			RenderAxis(XMMATRIX& tm, float scale) override;
	void			RenderScaler(XMMATRIX& tm, float scale) override;
	void			RenderRotator(XMMATRIX& tm, float scale) override;
	void			RenderMover(XMMATRIX& tm, float scale) override;
	void			RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color) override;
	void			RenderWorldGrid(XMMATRIX& mtWorld, int size, int segmentCount) override;
	void			RenderText(RENDER_TEXT_QUAD* pText) override;

	//////////////////////////////////////////////////////////////////////////
	//	IEngineRenderer
private:	
	bool								InitAsyncRenderThreadObjects() override;
	virtual void						AsyncRender(CCAMERA_DESC* pCameraDesc, IRenderingCallback* pRenderCallback) override;
	CCommandBuffer<eRENDER_COMMAND>*	GetFillCommandQueue() override	{	return &m_CommandQueue[m_FillBufferID]; }

private:
	void			RT_SignalFlushFinshed();
	unsigned int	RT_RenderThreadProc();
	void			RT_ProcessCommand();
					
	void			FlushAndWait();
	void			FlushWithoutWait();
	void			WaitUntilFlushFinished();
					
	bool			IsRenderThread();
	bool			IsMainThread();


private:	
	HANDLE								m_hRenderThread;

	UINT								m_RenderThreadID;
	UINT								m_MainThreadID;

	CCommandBuffer<eRENDER_COMMAND>		m_CommandQueue[NUM_RENDER_BUFFER];
	int									m_FillBufferID;
	int									m_ProcBufferID;

	BOOL								m_bDone;
	bool								m_bReadyToFlush;
};



