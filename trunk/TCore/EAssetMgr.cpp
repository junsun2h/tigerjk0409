#include "EAssetMgr.h"
#include "EWinFileLoader.h"
#include "ETextureDataProcessor.h"
#include <locale>



EAssetMgr::EAssetMgr()
	: m_pRDevice(NULL)
{

}

EAssetMgr::~EAssetMgr()
{

}

void EAssetMgr::Init( UINT numProcessThread, IRDevice*	pRDevice )
{
	m_pRDevice = pRDevice;
	m_AsyncLoader.Init( numProcessThread, this);
}

const IResource* EAssetMgr::GetResource( RESOURCE_TYPE type, long id )
{
	return NULL;
}

const IResource* EAssetMgr::GetResource( RESOURCE_TYPE type, std::string name )
{
	return GetResource(type, GET_HASH_KEY(name) );
}


long EAssetMgr::Load(char* fileName, RESOURCE_FILE_TYPE type, CALLBACK_LOAD_COMPLED pCallback, bool bAsync)
{
	RESOURCE_REQUEST request;
	request.pCallBackComplete = pCallback;
	request.pDataLoader = new EWinFileLoader(fileName);

	switch(type)
	{
	case RESOURCE_FILE_ACTOR:
		break;
	case RESOURCE_FILE_MOTION:
		break;
	case RESOURCE_FILE_TEXTURE:
		request.pDataProcessor = new ETextureDataProcessor( m_pRDevice, fileName );
		break;
	case RESOURCE_FILE_SHADER:
		break;
	case RESOURCE_FILE_MATERIAL:
		break;
	default:
		WCHAR szMessage[MAX_PATH];
		swprintf_s( szMessage, MAX_PATH, L"invalid resource file type %s\n", fileName );
		OutputDebugString( szMessage );
		SAFE_DELETE(request.pDataLoader);
		assert(0);
	}

	m_AsyncLoader.AddWorkItem(request);

	return GET_HASH_KEY(fileName);
}

void EAssetMgr::LoadCompletedResource( IResource* pResource)
{
	RESOURCE_TYPE type = pResource->Type();

	if( m_Resources[type].Lookup(pResource->RID) != NULL )
	{
		assert(0);
		return;
	}

	pResource->state = RESOURCE_LOAD_FINISHED;
	m_Resources[type].SetAt( pResource->RID, pResource );
}

void EAssetMgr::Clear()
{
	for( int i=0 ; i< NUM_RESOURCE_TYPE ; ++i)
	{
		POSITION pos = m_Resources[i].GetStartPosition();
		TYPE_RESOURCE_MAP::CPair* itr = NULL;

		while (pos)
		{
			itr = m_Resources[i].GetNext(pos);
			SAFE_DELETE( itr->m_value );
		}
	}
}