#include <process.h>

#include "CResource.h"
#include "CGrowableArray.h"

#include "IAssetMgr.h"
#include "IRDevice.h"
#include "ILoader.h"

#include "EGlobal.h"
#include "EWinFileLoader.h"
#include "EMeshDataProcessor.h"
#include "EActorDataProcessor.h"
#include "EMotionDataProcessor.h"
#include "ETextureDataProcessor.h"

#include "ELoader.h"


struct RESOURCE_REQUEST
{
	EWinFileLoader*			pDataLoader;
	IDataProcessor*			pDataProcessor;
	CResourceBase*			pResource;
	CALLBACK_LOAD_COMPLED	pCallBackComplete;

	RESOURCE_REQUEST()
		: pDataLoader(NULL)
		, pDataProcessor(NULL)
		, pResource(NULL)
		, pCallBackComplete(NULL)
	{
	}
};

CGrowableArray <RESOURCE_REQUEST>	g_IOQueue;
CGrowableArray <RESOURCE_REQUEST>	g_ProcessQueue;
CGrowableArray <RESOURCE_REQUEST>	g_MainThreadQueue;


CObjectPool<EWinFileLoader>			g_MemPoolFileLoader(100);
CObjectPool<EActorDataProcessor>	g_MemPoolActorDataProcessor(100);
CObjectPool<EMeshDataProcessor>		g_MemPoolMeshDataProcessor(100);
CObjectPool<EMotionDataProcessor>	g_MemPoolMotionDataProcessor(100);
CObjectPool<ETextureDataProcessor>	g_MemPoolTextureDataProcessor(100);


void RemoveResourceRquest(RESOURCE_REQUEST& resourceRequest)
{
	if( resourceRequest.pDataLoader)
		g_MemPoolFileLoader.Remove( resourceRequest.pDataLoader );

	if( resourceRequest.pDataProcessor )
	{
		eRESOURCE_FILE_TYPE fileType = resourceRequest.pDataProcessor->Type();

		if( fileType == RESOURCE_FILE_ACTOR ) 		g_MemPoolActorDataProcessor.Remove( resourceRequest.pDataProcessor );
		else if( fileType == RESOURCE_FILE_MESH ) 	g_MemPoolMeshDataProcessor.Remove( resourceRequest.pDataProcessor );
		else if( fileType == RESOURCE_FILE_MOTION )	g_MemPoolMotionDataProcessor.Remove( resourceRequest.pDataProcessor );
		else if( fileType == RESOURCE_FILE_TEXTURE ) g_MemPoolTextureDataProcessor.Remove( resourceRequest.pDataProcessor );
		else
			assert(0);
	}
}

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
	GetCurrentDirectoryA( MAX_PATH, m_Path);
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

	while( !m_bDone )
	{
		// Wait for a read or create request
		WaitForSingleObject( m_hIOQueueSemaphore, INFINITE );
		if( m_bDone )
			break;

		// Pop a request off of the IOQueue
		EnterCriticalSection( &m_csIOQueue );
		RESOURCE_REQUEST ResourceRequest = g_IOQueue.GetAt( 0 );
		g_IOQueue.Remove( 0 );
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
		g_ProcessQueue.Add( ResourceRequest );
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
		RESOURCE_REQUEST ResourceRequest = g_ProcessQueue.GetAt( 0 );
		g_ProcessQueue.Remove( 0 );
		LeaveCriticalSection( &m_csProcessQueue );

		// Decompress the data
		void* pData = NULL;
		SIZE_T cDataSize = 0;
		ResourceRequest.pDataLoader->GetData( &pData, &cDataSize );
		ResourceRequest.pDataProcessor->PT_Process( pData, cDataSize );

		if( FAILED( hr ) )
		{
			swprintf_s( szMessage, MAX_PATH, L"Processing Thread Error: hr = %x\n", hr );
			OutputDebugString( szMessage );
		}

		// Add it to the enderThreadQueue
		EnterCriticalSection( &m_csMainThreadQueue );
		g_MainThreadQueue.Add( ResourceRequest );
		LeaveCriticalSection( &m_csMainThreadQueue );
	}
	m_bProcessThreadDone = true;
	return 0;
}


//--------------------------------------------------------------------------------------
void ELoader::CompleteWork( UINT completeLimit)
{
	HRESULT hr = S_OK;

	EnterCriticalSection( &m_csMainThreadQueue );
	UINT numJobs = g_MainThreadQueue.GetSize();
	LeaveCriticalSection( &m_csMainThreadQueue );

	UINT offset = 0;

	for( UINT i = 0; i < numJobs && i < completeLimit; i++ )
	{
		EnterCriticalSection( &m_csMainThreadQueue );
		RESOURCE_REQUEST resourceRequest = g_MainThreadQueue.GetAt( offset );
		LeaveCriticalSection( &m_csMainThreadQueue );

		if( resourceRequest.pDataProcessor->CompleteWork() == false)
		{
			offset++;
			continue;
		}

		if( resourceRequest.pCallBackComplete != NULL )
			resourceRequest.pCallBackComplete(resourceRequest.pResource );
			
		EnterCriticalSection( &m_csMainThreadQueue );
		g_MainThreadQueue.Remove( offset );
		LeaveCriticalSection( &m_csMainThreadQueue );

				
		RemoveResourceRquest(resourceRequest);

		// Decrement num outstanding resources
		m_NumOustandingResources --;
	}
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


//--------------------------------------------------------------------------------------
CResourceBase* ELoader::Load(char* name, eRESOURCE_FILE_TYPE type, bool bForward)
{
	std::string fullPath = m_Path;
	
	if(RESOURCE_FILE_ACTOR == type )			fullPath = fullPath + "\\Data\\actor\\" + name + ".tac";
	else if( RESOURCE_FILE_MESH == type)		fullPath = fullPath + "\\Data\\mesh\\" + name + ".tmesh";
	else if(RESOURCE_FILE_MOTION == type )		fullPath = fullPath + "\\Data\\motion\\" + name + ".tmo";
	else if(RESOURCE_FILE_TEXTURE == type )		fullPath = fullPath + "\\Data\\texture\\" + name + ".dds";
	else if(RESOURCE_FILE_MATERIAL == type )	fullPath = fullPath + "\\Data\\material\\" + name + ".mtrl";

	return Load( fullPath.c_str(), name, type, bForward);
}


//--------------------------------------------------------------------------------------
CResourceBase* ELoader::Load(const char* fullpath, char* name, eRESOURCE_FILE_TYPE type, bool bForward)
{
	RESOURCE_REQUEST resourceRequest;
	resourceRequest.pDataLoader = g_MemPoolFileLoader.GetNew();
	resourceRequest.pDataLoader->SetFile(fullpath);

	if(RESOURCE_FILE_ACTOR == type )
	{
		resourceRequest.pResource = GLOBAL::AssetMgr()->CreateResource(RESOURCE_ACTOR, name);
		resourceRequest.pDataProcessor = g_MemPoolActorDataProcessor.GetNew();
	}
	else if( RESOURCE_FILE_MESH == type)
	{
		resourceRequest.pResource  = GLOBAL::AssetMgr()->CreateResource(RESOURCE_MESH, name);
		resourceRequest.pDataProcessor = g_MemPoolMeshDataProcessor.GetNew();
	}
	else if(RESOURCE_FILE_MOTION == type )
	{
		resourceRequest.pResource  = GLOBAL::AssetMgr()->CreateResource(RESOURCE_MOTION, name);
		resourceRequest.pDataProcessor = g_MemPoolMotionDataProcessor.GetNew();
	}
	else if(RESOURCE_FILE_TEXTURE == type )
	{
		resourceRequest.pResource  = GLOBAL::AssetMgr()->CreateResource(RESOURCE_TEXTURE, name);
		resourceRequest.pDataProcessor = g_MemPoolTextureDataProcessor.GetNew();
	}
	else if(RESOURCE_FILE_MATERIAL == type )
	{

	}

	resourceRequest.pDataProcessor->Init(resourceRequest.pResource, bForward);

	if( bForward == true)
	{
		void* pData;
		SIZE_T size;

		resourceRequest.pDataLoader->Load();
		resourceRequest.pDataLoader->GetData(&pData , &size);
		resourceRequest.pDataProcessor->Process(pData, size);
		resourceRequest.pDataProcessor->CompleteWork();

		RemoveResourceRquest(resourceRequest);
	}
	else
	{
		EnterCriticalSection( &m_csIOQueue );
		g_IOQueue.Add( resourceRequest );
		LeaveCriticalSection( &m_csIOQueue );

		m_NumOustandingResources++;

		// Signal that we have something to read
		ReleaseSemaphore( m_hIOQueueSemaphore, 1, NULL );
	}

	return resourceRequest.pResource;
}
