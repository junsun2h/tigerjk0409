#include "ELoader.h"
#include "EEngine.h"
#include <process.h>





//--------------------------------------------------------------------------------------
unsigned int WINAPI _FileIOThreadProc( LPVOID lpParameter )
{
	return ( ( ELoader* )lpParameter )->IOT_FileIOThreadProc();
}

//--------------------------------------------------------------------------------------
unsigned int WINAPI _ProcessingThreadProc( LPVOID lpParameter )
{
	return ( ( ELoader* )lpParameter )->PT_ProcessingThreadProc();
}

UINT ELoader::m_IOThreadID = 0;
UINT ELoader::m_ProcessThreadID[MAX_DATA_PROC_THREAD] = {0};
UINT ELoader::m_NumProcessingThreads = 0;


//--------------------------------------------------------------------------------------
ELoader::ELoader() 
	: m_bDone( false ),
	m_bProcessThreadDone( false ),
	m_bIOThreadDone( false ),
	m_NumOustandingResources( 0 ),
	m_hIOQueueSemaphore( 0 ),
	m_hProcessQueueSemaphore( 0 ),
	m_hIOThread( 0 )
{
}

//--------------------------------------------------------------------------------------
ELoader::~ELoader()
{
	DestroyAsyncLoadingThreadObjects();
}



//--------------------------------------------------------------------------------------
// Wait for all work in the queues to finish
//--------------------------------------------------------------------------------------
void ELoader::WaitForAllItems()
{
	CompleteWork( UINT_MAX );

	for(; ; )
	{
		// Only exit when all resources are loaded
		if( 0 == m_NumOustandingResources )
			return;

		// Service Queues
		CompleteWork( UINT_MAX );
		Sleep( 100 );
	}
}

//--------------------------------------------------------------------------------------
unsigned int ELoader::IOT_FileIOThreadProc()
{
	WCHAR szMessage[MAX_PATH];
	HRESULT hr = S_OK;
	m_bIOThreadDone = false;

	RESOURCE_REQUEST ResourceRequest = {0};

	while( !m_bDone )
	{
		// Wait for a read or create request
		WaitForSingleObject( m_hIOQueueSemaphore, INFINITE );
		if( m_bDone )
			break;

		// Pop a request off of the IOQueue
		EnterCriticalSection( &m_csIOQueue );
		ResourceRequest = m_IOQueue.GetAt( 0 );
		m_IOQueue.Remove( 0 );
		LeaveCriticalSection( &m_csIOQueue );

		// Handle a read request
		// Load the data
		hr = ResourceRequest.pDataLoader->IOT_Load();

		if( FAILED( hr ) )
		{
			swprintf_s( szMessage, MAX_PATH, L"FileIO Error: hr = %x\n", hr );
			OutputDebugString( szMessage );
		}

		// Add it to the ProcessQueue
		EnterCriticalSection( &m_csProcessQueue );
		m_ProcessQueue.Add( ResourceRequest );
		LeaveCriticalSection( &m_csProcessQueue );

		// Let the process thread know it's got work to do
		ReleaseSemaphore( m_hProcessQueueSemaphore, 1, NULL );
	}
	m_bIOThreadDone = true;
	return 0;
}

//--------------------------------------------------------------------------------------
unsigned int ELoader::PT_ProcessingThreadProc()
{
	WCHAR szMessage[MAX_PATH];
	HRESULT hr = S_OK;
	m_bProcessThreadDone = false;
	while( !m_bDone )
	{
		// Acquire ProcessQueueSemaphore
		WaitForSingleObject( m_hProcessQueueSemaphore, INFINITE );
		if( m_bDone )
			break;

		// Pop a request off of the ProcessQueue
		EnterCriticalSection( &m_csProcessQueue );
		RESOURCE_REQUEST ResourceRequest = m_ProcessQueue.GetAt( 0 );
		m_ProcessQueue.Remove( 0 );
		LeaveCriticalSection( &m_csProcessQueue );

		// Decompress the data
		void* pData = NULL;
		SIZE_T cDataSize = 0;
		hr = ResourceRequest.pDataLoader->GetData( &pData, &cDataSize );
		if( SUCCEEDED( hr ) )
		{
			// Process the data
			hr = ResourceRequest.pDataProcessor->PT_Process( pData, cDataSize );
		}

		if( FAILED( hr ) )
		{
			swprintf_s( szMessage, MAX_PATH, L"Processing Thread Error: hr = %x\n", hr );
			OutputDebugString( szMessage );
		}

		// Add it to the RenderThreadQueue
		EnterCriticalSection( &m_csMainThreadQueue );
		m_MainThreadQueue.Add( ResourceRequest );
		LeaveCriticalSection( &m_csMainThreadQueue );
	}
	m_bProcessThreadDone = true;
	return 0;
}

//--------------------------------------------------------------------------------------
bool ELoader::Init( UINT NumProcessingThreads )
{
	if( NumProcessingThreads > MAX_DATA_PROC_THREAD )
		NumProcessingThreads =  MAX_DATA_PROC_THREAD;

	LONG MaxSemaphoreCount = LONG_MAX;

	// Create 2 semaphores
	m_hIOQueueSemaphore = CreateSemaphore( NULL, 0, MaxSemaphoreCount, NULL );
	m_hProcessQueueSemaphore = CreateSemaphore( NULL, 0, MaxSemaphoreCount, NULL );

	// Create the queue critical sections
	InitializeCriticalSection( &m_csIOQueue );
	InitializeCriticalSection( &m_csProcessQueue );
	InitializeCriticalSection( &m_csMainThreadQueue );

	// Create the Processing threads
	m_NumProcessingThreads = NumProcessingThreads;
	if( !m_phProcessThreads )
		return false;

	for( UINT i = 0; i < m_NumProcessingThreads; i++ )
	{
		m_phProcessThreads[i] = ( HANDLE )_beginthreadex( NULL, 0, _ProcessingThreadProc, ( LPVOID )this,
			CREATE_SUSPENDED, &m_ProcessThreadID[i] );
		// we would set thread affinity here if we wanted to lock this thread to a processor
		ResumeThread( m_phProcessThreads[i] );
	}

	// Create the IO thread
	m_hIOThread = ( HANDLE )_beginthreadex( NULL, 0, _FileIOThreadProc, ( LPVOID )this, CREATE_SUSPENDED, &m_IOThreadID );
	// we would set thread affinity here if we wanted to lock this thread to a processor
	ResumeThread( m_hIOThread );

	return true;
}

//--------------------------------------------------------------------------------------
void ELoader::DestroyAsyncLoadingThreadObjects()
{
	m_bDone = true;

	ReleaseSemaphore( m_hIOQueueSemaphore, 1, NULL );
	ReleaseSemaphore( m_hProcessQueueSemaphore, 1, NULL );

	CloseHandle( m_hIOQueueSemaphore );
	CloseHandle( m_hProcessQueueSemaphore );

	DeleteCriticalSection( &m_csIOQueue );
	DeleteCriticalSection( &m_csProcessQueue );
	DeleteCriticalSection( &m_csMainThreadQueue );

	for( UINT i = 0; i < m_NumProcessingThreads; i++ )
		CloseHandle( m_phProcessThreads[i] );

	CloseHandle( m_hIOThread );
}

//--------------------------------------------------------------------------------------
void ELoader::CompleteWork( UINT CurrentNumResourcesToService)
{
	HRESULT hr = S_OK;

	EnterCriticalSection( &m_csMainThreadQueue );
	UINT numJobs = m_MainThreadQueue.GetSize();
	LeaveCriticalSection( &m_csMainThreadQueue );

	for( UINT i = 0; i < numJobs && i < CurrentNumResourcesToService; i++ )
	{
		EnterCriticalSection( &m_csMainThreadQueue );
		RESOURCE_REQUEST ResourceRequest = m_MainThreadQueue.GetAt( 0 );
		m_MainThreadQueue.Remove( 0 );
		LeaveCriticalSection( &m_csMainThreadQueue );

		ResourceRequest.pDataProcessor->PopData();

		if( ResourceRequest.pCallBackComplete != NULL )
			ResourceRequest.pCallBackComplete();

		SAFE_DELETE( ResourceRequest.pDataLoader );
		SAFE_DELETE( ResourceRequest.pDataProcessor );

		// Decrement num oustanding resources
		m_NumOustandingResources --;
	}
}

bool ELoader::IsIOThread()
{
	if (::GetCurrentThreadId() == m_IOThreadID )
		return true;
	return false;
}

bool ELoader::IsDataProcThread()
{
	UINT currentID = ::GetCurrentThreadId();

	for( UINT i= 0; i < m_NumProcessingThreads; ++i)
	{
		if( currentID == m_ProcessThreadID[i] )
			return true;
	}
	return false;
}



CResourceBase* ELoader::LoadForward(char* fileName, char* name, RESOURCE_FILE_TYPE type)
{
	if(RESOURCE_FILE_ACTOR == type )
	{

	}
	else if(RESOURCE_FILE_MOTION == type )
	{

	}
	else if(RESOURCE_FILE_TEXTURE == type )
	{
		CResourceTexture* pTexture = g_Engine.RDevice()->CreateTextureFromFile(fileName);
		strcpy_s( pTexture->name, name );

		g_Engine.AssetMgr()->Insert(pTexture);
		return pTexture;
	}
	else if(RESOURCE_FILE_MATERIAL == type )
	{

	}

	return NULL;
}


//--------------------------------------------------------------------------------------
// Add a work item to the queue of work items
//--------------------------------------------------------------------------------------
void ELoader::LoadBackword(char* fileName, char* name, RESOURCE_FILE_TYPE type)
{
	RESOURCE_REQUEST resourceRequest;

	if( !resourceRequest.pDataLoader || !resourceRequest.pDataProcessor )
	{
		assert(0);
		return;
	}

	// Add the request to the read queue
	EnterCriticalSection( &m_csIOQueue );
	m_IOQueue.Add( resourceRequest );
	LeaveCriticalSection( &m_csIOQueue );

	// TODO: critsec around this?
	m_NumOustandingResources ++;

	// Signal that we have something to read
	ReleaseSemaphore( m_hIOQueueSemaphore, 1, NULL );
}