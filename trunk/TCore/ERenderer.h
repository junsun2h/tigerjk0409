#pragma once
#include "CDefine.h"
#include "IRDevice.h"
#include "CGrowableArray.h"


enum eRENDER_BUFFER
{
	RENDER_BUFFER_0,
	RENDER_BUFFER_1,
	NUM_RENDER_BUFFER,
};

class ERenderer
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
	ERenderer();
	~ERenderer();

	bool                        InitAsyncRenderThreadObjects();
	void						WaitUntilFlushFinished();
private:
	void						RT_SignalFlushFinshed();

private:
	unsigned int                RT_RenderThreadProc();
	void		                RT_ProcessCommand();

	friend unsigned int WINAPI  _RenderThreadProc( LPVOID lpParameter );

	bool						IsRenderThread();
	bool						IsMainThread();

	byte*						AddCommand(eRENDER_COMMAND cmd, size_t bufBytes);

	void						FlushAndWait();
	void						FlushWithoutWait();

private:	
	_inline void	AddDWORD(byte*& ptr, uint32 nVal);
};



_inline void ERenderer::AddDWORD(byte*& ptr, uint32 nVal)
{
	*(uint32*)ptr = nVal;
	ptr += sizeof(uint32);
}