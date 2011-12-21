#include "EAssetMgr.h"
#include "EWinFileLoader.h"
#include "ETextureDataProcessor.h"
#include <locale>


const IResource* EAssetMgr::GetResource( RESOURCE_TYPE type, long id )
{
	return NULL;
}

const IResource* EAssetMgr::GetResource( RESOURCE_TYPE type, std::string name )
{
	return GetResource(type, GET_RID(name) );
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

	m_AsyncLoader.MT_AddWorkItem(request);

	return GET_RID(fileName);
}

void EAssetMgr::LoadCompletedResource( IResource* pResource)
{
	RESOURCE_TYPE type = pResource->Type();

	if( m_Resources[type].find(pResource->RID) == m_Resources[type].end() )
	{
		assert(0);
		return;
	}

	pResource->state = RESOURCE_LOAD_FINISHED;
	m_Resources[type].insert( TYPE_RESOURCE_MAP::value_type( pResource->RID, pResource ) );
}