#include "EGlobal.h"
#include "EAssetMgr.h"


//--------------------------------------------------------------------------------------------------------
EAssetMgr::EAssetMgr()
{
}

EAssetMgr::~EAssetMgr()
{

}

CResourceBase* EAssetMgr::UnSafeGetResource( eRESOURCE_TYPE type, long id )
{
	TYPE_RESOURCE_MAP::CPair* itr = m_Resources[type].Lookup( id );
	if( itr != NULL)
		return itr->m_value;

	return NULL;
}

CResourceBase* EAssetMgr::UnSafeGetResource( eRESOURCE_TYPE type, const char* name )
{
	return UnSafeGetResource(type, GET_HASH_KEY(name) );
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

	if( type == RESOURCE_GEOMETRY || type == RESOURCE_TEXTURE )
		GLOBAL::RDevice()->CreateGraphicBuffer( pResource );

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
	IRDevice*		pRDevice		= GLOBAL::RDevice();
	IResourceMemMgr* pMemeoryPool	= GLOBAL::ResourceMemMgr();

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
				pMemeoryPool->Remove( pGeometry );
				m_Resources[RESOURCE_GEOMETRY].RemoveKey( key);
			}
		}
	}
	else if( type == RESOURCE_TEXTURE )
	{
		pRDevice->RemoveGraphicBuffer(pResource);
	}

	pMemeoryPool->Remove( pResource );
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
