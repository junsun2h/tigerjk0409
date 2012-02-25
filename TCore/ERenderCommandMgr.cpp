#include <process.h>

#include "CResource.h"
#include "CGrowableArray.h"
#include "CEngineParam.h"
#include "CCamera.h"
#include "CCommandBuffer.h"

#include "IRenderHelper.h"
#include "IEntityProxy.h"
#include "IRDevice.h"
#include "IRenderCommand.h"
#include "ISpaceMgr.h"

#include "EGlobal.h"
#include "ERenderCommandMgr.h"


#define CHECK_COMMAND(ptr)	\
	assert(  ( ptr - m_CommandFill.GetData() ) == m_CommandFill.GetSize()  );

//--------------------------------------------------------------------------------------
unsigned int WINAPI _RenderThreadProc( LPVOID lpParameter )
{
	return ( ( ERenderCommandMgr* )lpParameter )->RT_RenderThreadProc();
}

ERenderCommandMgr::ERenderCommandMgr()
{
	m_FillBufferID  = 0;
	m_ProcBufferID  = 1;
}

ERenderCommandMgr::~ERenderCommandMgr()
{

}

//--------------------------------------------------------------------------------------
bool ERenderCommandMgr::InitAsyncRenderThreadObjects( )
{
	m_MainThreadID = ::GetCurrentThreadId();
	
	// Create the thread
	m_hRenderThread = ( HANDLE )_beginthreadex( NULL, 0, _RenderThreadProc, ( LPVOID )this, CREATE_SUSPENDED, &m_RenderThreadID );
	// we would set thread affinity here if we wanted to lock this thread to a processor
	ResumeThread( m_hRenderThread );

	return true;
}


//--------------------------------------------------------------------------------------
bool ERenderCommandMgr::IsRenderThread()
{
	if (::GetCurrentThreadId() == m_RenderThreadID )
		return true;
	return false;
}


//--------------------------------------------------------------------------------------
bool ERenderCommandMgr::IsMainThread()
{
	if (::GetCurrentThreadId() == m_MainThreadID )
		return true;
	return false;
}


//--------------------------------------------------------------------------------------
unsigned int ERenderCommandMgr::RT_RenderThreadProc()
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
void ERenderCommandMgr::RT_ProcessCommand()
{
	IRenderHelper* pRenderHelper = GLOBAL::RDevice()->GetRenderHelper();
	IRenderStrategy* pRenderer = GLOBAL::RDevice()->GetRenderer();

	assert (IsRenderThread());

	if ( m_bReadyToFlush == false )
		return;
	CCommandBuffer<eRENDER_COMMAND>* pQueue = &m_CommandQueue[m_ProcBufferID];

	while ( !pQueue->IsEmpty() )
	{
		eRENDER_COMMAND cmd = pQueue->PopCommandStart();

		switch(cmd)
		{
		case RC_RenderFrame:
			{
				CCAMERA_DESC* pParam;
				pQueue->PopParam(pParam);
				pRenderer->RenderFrame(pParam);
			}break;
		case RC_DRAW_OBJECT:
			{
				CRenderParamSkin* pParam;
				pQueue->PopParam(pParam);
				pRenderer->SetMaterial( pParam->pMaterial, pParam->pGeometry);
				pRenderer->SetTransform( pParam->worldTM );
				pRenderer->RenderGeometry( pParam->pGeometry);
			}break;
		case RC_DRAW_OBJECT_SKIN:
			{
				CRenderParamSkin* pParam;
				pQueue->PopParam(pParam);
				pRenderer->SetMaterial( pParam->pMaterial, pParam->pGeometry);
				pRenderer->SetTransform( pParam->worldTM );
				pRenderer->SetJointTransforms( pParam->refSkinTM, pParam->refSkinTMCount );
				pRenderer->RenderGeometry( pParam->pGeometry);
			}break;
		case RC_DRAW_HELPER_Skeleton:
			{

			}break;
		case RC_DRAW_HELPER_Axis:
			{

			}break;
		case RC_DRAW_HELPER_Scaler:
			{

			}break;
		case RC_DRAW_HELPER_Rotator:
			{

			}break;
		case RC_DRAW_HELPER_Mover:
			{

			}break;
		case RC_DRAW_HELPER_Box:
			{

			}break;
		case RC_DRAW_HELPER_WorldGrid:
			{

			}break;
		case RC_DRAW_HELPER_Text:
			{

			}break;
		default:
			assert(0);
		}

		pQueue->PopCommandEnd();
	}

	GLOBAL::RDevice()->Present();

	m_bReadyToFlush = false;
}


//--------------------------------------------------------------------------------------
void ERenderCommandMgr::FlushAndWait()
{
	if ( IsRenderThread() )
		return;

	WaitUntilFlushFinished();

	int tempBufferID= m_ProcBufferID;
	m_ProcBufferID = m_FillBufferID;
	m_bReadyToFlush = true;

	WaitUntilFlushFinished();

	m_ProcBufferID = tempBufferID;
}


//--------------------------------------------------------------------------------------
void ERenderCommandMgr::FlushWithoutWait()
{
	assert (IsMainThread());

	WaitUntilFlushFinished();

	//swap Fill & Proc Buffer ID
	m_ProcBufferID = m_FillBufferID;
	m_FillBufferID = !m_ProcBufferID;

	m_bReadyToFlush = true;
}


//--------------------------------------------------------------------------------------
void ERenderCommandMgr::WaitUntilFlushFinished()
{
	HWND hwnd = GLOBAL::RDevice()->GetHWND();

	while(m_bReadyToFlush)
	{
#ifdef WIN32
		if ( hwnd )
		{
			MSG msg;
			while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
#endif
	}
}

//--------------------------------------------------------------------------------------
void ERenderCommandMgr::AsyncRender(CCAMERA_DESC* pCameraDesc, IRenderingCallback* pRenderCallback)
{
	FlushWithoutWait();

	if( m_CommandQueue[m_FillBufferID].IsEmpty() == false)
	{
		assert(0);
		return;
	}

	m_CommandQueue[m_FillBufferID].AddCommandStart(RC_RenderFrame);
	m_CommandQueue[m_FillBufferID].AddParam(pCameraDesc);
	m_CommandQueue[m_FillBufferID].AddCommandEnd();

//	if( pRenderCallback )
//		pRenderCallback->PreRender();

	GLOBAL::Engine()->SpaceMgr()->Render();

//	if( pRenderCallback )
//		pRenderCallback->PostRender();
}
