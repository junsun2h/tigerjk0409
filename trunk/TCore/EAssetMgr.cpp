#include "EAssetMgr.h"
#include "EWinFileLoader.h"
#include "ETextureDataProcessor.h"
#include "EEngine.h"
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

const CResourceBase* EAssetMgr::GetResource( RESOURCE_TYPE type, long id )
{
	return NULL;
}

const CResourceBase* EAssetMgr::GetResource( RESOURCE_TYPE type, std::string name )
{
	return GetResource(type, GET_HASH_KEY(name) );
}


long EAssetMgr::LoadFromFile(char* fileName, RESOURCE_FILE_TYPE type, CALLBACK_LOAD_COMPLED pCallback, bool bAsync)
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

long EAssetMgr::LoadForward( CResourceBase* pResource )
{
	g_Engine.RDevice()->CreateGraphicBuffer( pResource );
	return LoadComplete( pResource );
}

long EAssetMgr::LoadComplete( CResourceBase* pResource)
{
	RESOURCE_TYPE type = pResource->Type();

	if( m_Resources[type].Lookup(pResource->RID) == NULL )
		pResource->RID = GET_HASH_KEY( pResource->name );

	pResource->state = RESOURCE_LOAD_FINISHED;
	m_Resources[type].SetAt( pResource->RID, pResource );

	return pResource->RID;
}

void EAssetMgr::Clear()
{
	for( int i = 0; i < NUM_RESOURCE_TYPE; ++i )
	{
		if( i == RESOURCE_GEOMETRY )
			continue; // it will be deleted when deletes mesh.

		POSITION pos = m_Resources[i].GetStartPosition();
		TYPE_RESOURCE_MAP::CPair* itr = NULL;

		while (pos)
		{
			itr = m_Resources[i].GetNext(pos);
			Remove( RESOURCE_TYPE(i), itr->m_key );
		}
	}
}

void EAssetMgr::Remove(RESOURCE_TYPE type, long id)
{
	if( type == RESOURCE_MESH )
	{
		CResourceMesh* pMesh = (CResourceMesh*)m_Resources[type].Lookup( id )->m_value;
		for(int i=0; i< pMesh->geometryNum; ++i)
		{
			int key = pMesh->goemetries[i];
			TYPE_RESOURCE_MAP::CPair* itr = m_Resources[RESOURCE_GEOMETRY].Lookup( key );

			if( itr != NULL)
			{
				CResourceGeometry* pGeometry = (CResourceGeometry*)itr->m_value;
				g_Engine.RDevice()->RemoveGraphicBuffer(pGeometry);
				SAFE_DELETE( pGeometry );
				m_Resources[RESOURCE_GEOMETRY].RemoveKey( key);
			}
		}
	}

	SAFE_DELETE(m_Resources[type].Lookup( id )->m_value);
	m_Resources[type].RemoveKey( id);
}

void EAssetMgr::Remove(RESOURCE_TYPE type, std::string& name)
{
	Remove(type, GET_HASH_KEY(name));
}