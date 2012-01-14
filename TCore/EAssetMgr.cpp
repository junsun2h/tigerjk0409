#include "EAssetMgr.h"
#include "EWinFileLoader.h"
#include "ETextureDataProcessor.h"
#include "EEngine.h"
#include <locale>



EAssetMgr::EAssetMgr()
{
	memset( m_DefferdRenderTargets, 0, 4*NUM_DEFFERED_RENDER_TARGET );
}

EAssetMgr::~EAssetMgr()
{

}

void EAssetMgr::Init( const CENGINE_INIT_PARAM &param)
{
	m_AsyncLoader.Init( param.numOfProcessThread, this);
	
	CResourceTexture* pGeometryTexture = new CResourceTexture;
	strcpy_s( pGeometryTexture->name, "GeometryRenderTarget" );
	pGeometryTexture->height = param.height;
	pGeometryTexture->Width = param.width;
	pGeometryTexture->usage = TEXTURE_RENDER_RAGET;
	pGeometryTexture->Format = COLOR_FORMAT_R10G10B10A2_UNORM;
	pGeometryTexture->MipLevels = 1;

	LoadForward( pGeometryTexture );
	m_DefferdRenderTargets[RENDER_TARGET_GEOMERTY] = pGeometryTexture;

	CResourceTexture* pLightTexture = new CResourceTexture;
	strcpy_s( pLightTexture->name, "LightRenderTarget" );
	pLightTexture->height = param.height;
	pLightTexture->Width = param.width;
	pLightTexture->usage = TEXTURE_RENDER_RAGET;
	pLightTexture->Format = COLOR_FORMAT_R8G8B8A8_UNORM;
	pLightTexture->MipLevels = 1;

	LoadForward( pLightTexture );
	m_DefferdRenderTargets[RENDER_TARGET_LIGHT] = pLightTexture;
}


void EAssetMgr::ResizeDefferedRenderTarget(UINT width, UINT height)
{
	IRDevice* pRDevice = g_Engine.RDevice();
	
	for( int i=0; i < NUM_DEFFERED_RENDER_TARGET; ++i)
	{
		pRDevice->RemoveGraphicBuffer( m_DefferdRenderTargets[i] );
		m_DefferdRenderTargets[i]->Width = width;
		m_DefferdRenderTargets[i]->height = height;
		pRDevice->CreateGraphicBuffer( m_DefferdRenderTargets[i] );
	}
}

const CResourceBase* EAssetMgr::GetResource( RESOURCE_TYPE type, long id )
{
	TYPE_RESOURCE_MAP::CPair* itr = m_Resources[RESOURCE_GEOMETRY].Lookup( id );
	if( itr != NULL)
		return itr->m_value;

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
		request.pDataProcessor = new ETextureDataProcessor( fileName );
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
	IRDevice* pRDevice = g_Engine.RDevice();
	CResourceBase* pResource = m_Resources[type].Lookup( id )->m_value;

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
				pRDevice->RemoveGraphicBuffer(pGeometry);
				SAFE_DELETE( pGeometry );
				m_Resources[RESOURCE_GEOMETRY].RemoveKey( key);
			}
		}
	}
	else if( type == RESOURCE_TEXTURE )
	{
		pRDevice->RemoveGraphicBuffer(pResource);
	}

	SAFE_DELETE(pResource);
	m_Resources[type].RemoveKey( id);
}

void EAssetMgr::Remove(RESOURCE_TYPE type, std::string& name)
{
	Remove(type, GET_HASH_KEY(name));
}