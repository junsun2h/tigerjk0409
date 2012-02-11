#include "ERenderer.h"
#include <process.h>


#define CHECK_COMMAND(ptr)	\
	assert(  ( ptr - m_CommandFill.GetData() ) == m_CommandFill.GetSize()  );

//--------------------------------------------------------------------------------------
unsigned int WINAPI _RenderThreadProc( LPVOID lpParameter )
{
	return ( ( ERenderer* )lpParameter )->RT_RenderThreadProc();
}

ERenderer::ERenderer()
{
	m_FillBufferID  = 0;
	m_ProcBufferID  = 1;
}

ERenderer::~ERenderer()
{

}

//--------------------------------------------------------------------------------------
bool ERenderer::InitAsyncRenderThreadObjects( )
{
	m_MainThreadID = ::GetCurrentThreadId();

	LONG MaxSemaphoreCount = LONG_MAX;
	m_hRenderQueueSemaphore = CreateSemaphore( NULL, 0, MaxSemaphoreCount, NULL );

	// Create the queue critical sections
	InitializeCriticalSection( &m_csRenderQueue );

	// Create the IO thread
	m_hRenderThread = ( HANDLE )_beginthreadex( NULL, 0, _RenderThreadProc, ( LPVOID )this, CREATE_SUSPENDED, &m_RenderThreadID );
	// we would set thread affinity here if we wanted to lock this thread to a processor
	ResumeThread( m_hRenderThread );

	return true;
}


//--------------------------------------------------------------------------------------
bool ERenderer::IsRenderThread()
{
	if (::GetCurrentThreadId() == m_RenderThreadID )
		return true;
	return false;
}


//--------------------------------------------------------------------------------------
bool ERenderer::IsMainThread()
{
	if (::GetCurrentThreadId() == m_MainThreadID )
		return true;
	return false;
}


//--------------------------------------------------------------------------------------
unsigned int ERenderer::RT_RenderThreadProc()
{
	while( !m_bDone )
	{
		// wait flush condition
		while (!m_bReadyToFlush)
		{
			if (m_bDone)break;
			::Sleep(0);
		}

		if(m_bDone)	
		{
			m_bReadyToFlush = false;
			break;//put it here to safely shut down
		}

		RT_ProcessCommand();
	}

	return 0;
}


//--------------------------------------------------------------------------------------
void ERenderer::RT_ProcessCommand()
{
	assert (IsRenderThread());
	
	if ( m_bReadyToFlush == false )
		return;

	int index = 0;
	byte *pP;
	while ( index < m_CommandQueue[m_ProcBufferID].GetSize() )
	{
		pP = &m_CommandQueue[m_ProcBufferID][index];
		index += sizeof(int);
		byte cmd = (byte)*((int *)pP);

		switch(cmd)
		{
		case RC_RESET_DEVICE:
			break;
		default:
			assert(0);
		}
	}

	m_bReadyToFlush = false;
}


//--------------------------------------------------------------------------------------
byte* ERenderer::AddCommand(eRENDER_COMMAND cmd, size_t bufBytes)
{
	assert (IsMainThread());

	byte* ptr = m_CommandQueue[m_FillBufferID].GrowSize( sizeof(uint32) + bufBytes ); 
	AddDWORD(ptr, cmd);
	return ptr;
}


//--------------------------------------------------------------------------------------
void ERenderer::FlushAndWait()
{
	if ( IsRenderThread() )
		return;

	WaitUntilFlushFinished();
	
	int tempBufferID= m_ProcBufferID;
	m_ProcBufferID = m_FillBufferID;
	m_bReadyToFlush = true;

	WaitUntilFlushFinished();

	m_ProcBufferID = tempBufferID;
	m_CommandQueue[m_FillBufferID].RemoveAll();
}


//--------------------------------------------------------------------------------------
void ERenderer::FlushWithoutWait()
{
	assert (IsMainThread());

	WaitUntilFlushFinished();

	//swap Fill & Proc Buffer ID
	m_ProcBufferID = m_FillBufferID;
	m_FillBufferID = !m_ProcBufferID;

	m_bReadyToFlush = true;
}


//--------------------------------------------------------------------------------------
void ERenderer::WaitUntilFlushFinished()
{
	while(m_bReadyToFlush)
	{
#ifdef WIN32
		if ( m_pRDevice->GetHWND() )
		{
			MSG msg;
			while (PeekMessage(&msg, m_pRDevice->GetHWND(), 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
#endif
	}
}
