#include <process.h>

#include "CResource.h"
#include "CGrowableArray.h"
#include "CEngineParam.h"
#include "CCamera.h"
#include "CCommandBuffer.h"
#include "CQuad.h"

#include "IRenderHelper.h"
#include "IEntityProxy.h"
#include "IRDevice.h"
#include "IRenderer.h"
#include "ISpaceMgr.h"

#include "EGlobal.h"
#include "ERenderer.h"


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

	// Create the thread
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
	IRenderHelper* pRenderHelper = GLOBAL::RDevice()->GetRenderHelper();
	IRenderStrategy* pRenderStrategy = GLOBAL::RDevice()->GetRenderStrategy();

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
				pRenderStrategy->RenderFrame(pParam);
			}break;
		case RC_DRAW_OBJECT:
			{
				CRenderParam* pParam;
				pQueue->PopParam(pParam);
				pRenderStrategy->SetMaterial( pParam->pMaterial, pParam->pGeometry);
				pRenderStrategy->SetTransform( pParam->worldTM );
				pRenderStrategy->RenderGeometry( pParam->pGeometry);
			}break;
		case RC_DRAW_OBJECT_SKIN:
			{
				CRenderParamSkin* pParam;
				pQueue->PopParam(pParam);
				pRenderStrategy->SetMaterial( pParam->pMaterial, pParam->pGeometry);
				pRenderStrategy->SetTransform( pParam->worldTM );
				pRenderStrategy->SetJointTransforms( pParam->refSkinTM, pParam->refSkinTMCount );
				pRenderStrategy->RenderGeometry( pParam->pGeometry);
			}break;
		case RC_DRAW_HELPER_Skeleton:
			{
				IEntity* pEntity;

				pQueue->PopAddress(pEntity);
				pRenderHelper->RenderSkeleton(pEntity);
			}break;
		case RC_DRAW_HELPER_Axis:
			{
				XMMATRIX* tm;
				float* scale;

				pQueue->PopParam(tm);
				pQueue->PopParam(scale);

				pRenderHelper->RenderAxis(*tm, *scale);
			}break;
		case RC_DRAW_HELPER_Scaler:
			{
				XMMATRIX* tm;
				float* scale;

				pQueue->PopParam(tm);
				pQueue->PopParam(scale);

				pRenderHelper->RenderScaler(*tm, *scale);
			}break;
		case RC_DRAW_HELPER_Rotator:
			{
				XMMATRIX* tm;
				float* scale;

				pQueue->PopParam(tm);
				pQueue->PopParam(scale);

				pRenderHelper->RenderRotator(*tm, *scale);
			}break;
		case RC_DRAW_HELPER_Mover:
			{
				XMMATRIX* tm;
				float* scale;

				pQueue->PopParam(tm);
				pQueue->PopParam(scale);

				pRenderHelper->RenderMover(*tm, *scale);
			}break;
		case RC_DRAW_HELPER_Box:
			{
				XMMATRIX* mtWorld;
				CVector3* min;
				CVector3* max;
				DWORD* color;

				pQueue->PopParam(mtWorld);
				pQueue->PopParam(min);
				pQueue->PopParam(max);
				pQueue->PopParam(color);

				pRenderHelper->RenderBox(*mtWorld, *min, *max, *color);
			}break;
		case RC_DRAW_HELPER_WorldGrid:
			{
				XMMATRIX* mtWorld;
				int* size;
				int* segmentCount;

				pQueue->PopParam(mtWorld);
				pQueue->PopParam(size);
				pQueue->PopParam(segmentCount);

				pRenderHelper->RenderWorldGrid( *mtWorld, *size, *segmentCount);
			}break;
		case RC_DRAW_HELPER_Text:
			{
				RENDER_TEXT_QUAD* pText;
				pQueue->PopParam(pText);

				pRenderHelper->RenderText(pText);
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
void ERenderer::AsyncRender(CCAMERA_DESC* pCameraDesc, IRenderingCallback* pRenderCallback)
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

	if( pRenderCallback )
		pRenderCallback->PreRender();

	GLOBAL::Engine()->SpaceMgr()->Render();

	if( pRenderCallback )
		pRenderCallback->PostRender();
}


void ERenderer::RenderSkeleton(IEntity* pEntity)
{
	m_CommandQueue[m_FillBufferID].AddCommandStart(RC_DRAW_HELPER_Skeleton);
	m_CommandQueue[m_FillBufferID].AddAddress(pEntity);
	m_CommandQueue[m_FillBufferID].AddCommandEnd();
}

void ERenderer::RenderAxis(XMMATRIX& tm, float scale)
{
	m_CommandQueue[m_FillBufferID].AddCommandStart(RC_DRAW_HELPER_Axis);
	m_CommandQueue[m_FillBufferID].AddParam(tm);
	m_CommandQueue[m_FillBufferID].AddParam(scale);
	m_CommandQueue[m_FillBufferID].AddCommandEnd();
}

void ERenderer::RenderScaler(XMMATRIX& tm, float scale)
{
	m_CommandQueue[m_FillBufferID].AddCommandStart(RC_DRAW_HELPER_Scaler);
	m_CommandQueue[m_FillBufferID].AddParam(tm);
	m_CommandQueue[m_FillBufferID].AddParam(scale);
	m_CommandQueue[m_FillBufferID].AddCommandEnd();
}

void ERenderer::RenderRotator(XMMATRIX& tm, float scale)
{
	m_CommandQueue[m_FillBufferID].AddCommandStart(RC_DRAW_HELPER_Rotator);
	m_CommandQueue[m_FillBufferID].AddParam(tm);
	m_CommandQueue[m_FillBufferID].AddParam(scale);
	m_CommandQueue[m_FillBufferID].AddCommandEnd();
}

void ERenderer::RenderMover(XMMATRIX& tm, float scale)
{
	m_CommandQueue[m_FillBufferID].AddCommandStart(RC_DRAW_HELPER_Mover);
	m_CommandQueue[m_FillBufferID].AddParam(tm);
	m_CommandQueue[m_FillBufferID].AddParam(scale);
	m_CommandQueue[m_FillBufferID].AddCommandEnd();
}

void ERenderer::RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color)
{
	m_CommandQueue[m_FillBufferID].AddCommandStart(RC_DRAW_HELPER_Box);
	m_CommandQueue[m_FillBufferID].AddParam(mtWorld);
	m_CommandQueue[m_FillBufferID].AddParam(min);
	m_CommandQueue[m_FillBufferID].AddParam(max);
	m_CommandQueue[m_FillBufferID].AddParam(color);
	m_CommandQueue[m_FillBufferID].AddCommandEnd();
}

void ERenderer::RenderWorldGrid(XMMATRIX& mtWorld, int size, int segmentCount)
{
	m_CommandQueue[m_FillBufferID].AddCommandStart(RC_DRAW_HELPER_WorldGrid);
	m_CommandQueue[m_FillBufferID].AddParam(mtWorld);
	m_CommandQueue[m_FillBufferID].AddParam(size);
	m_CommandQueue[m_FillBufferID].AddParam(segmentCount);
	m_CommandQueue[m_FillBufferID].AddCommandEnd();
}

void ERenderer::RenderText(RENDER_TEXT_QUAD* pText)
{
	m_CommandQueue[m_FillBufferID].AddCommandStart(RC_DRAW_HELPER_Text);
	m_CommandQueue[m_FillBufferID].AddParam(pText);
	m_CommandQueue[m_FillBufferID].AddCommandEnd();
}
