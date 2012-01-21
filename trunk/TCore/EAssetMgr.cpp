#include "EAssetMgr.h"
#include "EEngine.h"
#include <locale>


//--------------------------------------------------------------------------------------------------------
EAssetMgr::EAssetMgr()

{
	memset( m_DefferdRenderTargets, 0, 4*NUM_DEFFERED_RENDER_TARGET );
}

EAssetMgr::~EAssetMgr()
{

}

void EAssetMgr::Init( const CENGINE_INIT_PARAM &param)
{	
	IEngineMemoryMgr* pMemoryMgr = g_Engine.EngineMemoryMgr();
	
	CResourceTexture* pGeometryTexture = (CResourceTexture*)pMemoryMgr->GetNewResource(RESOURCE_TEXTURE);
	strcpy_s( pGeometryTexture->name, "GeometryRenderTarget" );
	pGeometryTexture->height = param.height;
	pGeometryTexture->Width = param.width;
	pGeometryTexture->usage = TEXTURE_RENDER_RAGET;
	pGeometryTexture->Format = COLOR_FORMAT_R8G8B8A8_UNORM;
	pGeometryTexture->MipLevels = 1;

	Insert( pGeometryTexture );
	m_DefferdRenderTargets[RENDER_TARGET_GEOMERTY] = pGeometryTexture;

	CResourceTexture* pLightTexture = (CResourceTexture*)pMemoryMgr->GetNewResource(RESOURCE_TEXTURE);
	strcpy_s( pLightTexture->name, "LightRenderTarget" );
	pLightTexture->height = param.height;
	pLightTexture->Width = param.width;
	pLightTexture->usage = TEXTURE_RENDER_RAGET;
	pLightTexture->Format = COLOR_FORMAT_R8G8B8A8_UNORM;
	pLightTexture->MipLevels = 1;

	Insert( pLightTexture );
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

const CResourceBase* EAssetMgr::GetResource( eRESOURCE_TYPE type, long id )
{
	TYPE_RESOURCE_MAP::CPair* itr = m_Resources[type].Lookup( id );
	if( itr != NULL)
		return itr->m_value;

	return NULL;
}

const CResourceBase* EAssetMgr::GetResource( eRESOURCE_TYPE type, const char* name )
{
	return GetResource(type, GET_HASH_KEY(name) );
}

const CResourceBase* EAssetMgr::GetResource( eRESOURCE_TYPE type, std::string name )
{
	return GetResource(type, GET_HASH_KEY(name) );
}

long EAssetMgr::Insert( CResourceBase* pResource)
{
	eRESOURCE_TYPE type = pResource->Type();
	
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
			continue; // it will be deleted when mesh is deleted.

		POSITION pos = m_Resources[i].GetStartPosition();
		TYPE_RESOURCE_MAP::CPair* itr = NULL;

		while (pos)
		{
			itr = m_Resources[i].GetNext(pos);
			Remove( eRESOURCE_TYPE(i), itr->m_key );
		}
	}
}

void EAssetMgr::Remove(eRESOURCE_TYPE type, long id)
{
	IRDevice*		pRDevice		= g_Engine.RDevice();
	EEngineMemoryMgr* pMemeoryPool	= g_Engine.EngMemoryPoolMgr();

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
				pMemeoryPool->RemoveResource( pGeometry );
				m_Resources[RESOURCE_GEOMETRY].RemoveKey( key);
			}
		}
	}
	else if( type == RESOURCE_TEXTURE )
	{
		pRDevice->RemoveGraphicBuffer(pResource);
	}

	pMemeoryPool->RemoveResource( pResource );
	m_Resources[type].RemoveKey( id);
}

void EAssetMgr::Remove(eRESOURCE_TYPE type, std::string& name)
{
	Remove(type, GET_HASH_KEY(name));
}

void EAssetMgr::Remove(eRESOURCE_TYPE type, const char* name)
{
	Remove(type, GET_HASH_KEY(name));
}
