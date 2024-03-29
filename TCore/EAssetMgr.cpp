#include "CResource.h"

#include "IRDevice.h"
#include "IAssetMgr.h"
#include "ILoader.h"

#include "EGlobal.h"
#include "EAssetMgr.h"


CObjectPool<CResourceGeometry>	g_MemPoolGeometry(100);
CObjectPool<CResourceMesh>		g_MemPoolMesh(100);
CObjectPool<CResourceTexture>	g_MemPoolTexture(100);
CObjectPool<CResourceActor>		g_MemPoolActor(100);
CObjectPool<CResourceMotion>	g_MemPoolMotion(100);
CObjectPool<CResourceMtrl>		g_MemPoolMtrl(100);


CResourceBase* MemGetNew(eRESOURCE_TYPE type)
{
	if( type == RESOURCE_GEOMETRY )			{	return g_MemPoolGeometry.GetNew();	}
	else if( type == RESOURCE_TEXTURE )		{	return g_MemPoolTexture.GetNew(); }
	else if( type == RESOURCE_MESH )		{	return g_MemPoolMesh.GetNew();	}
	else if( type == RESOURCE_ACTOR )		{	return g_MemPoolActor.GetNew();	}
	else if( type == RESOURCE_MOTION )		{	return g_MemPoolMotion.GetNew();	}
	else if( type == RESOURCE_MATERIAL )	{	return g_MemPoolMtrl.GetNew();	}

	assert(0);
	return NULL;
}

void MemRemove(CResourceBase* pResource)
{
	eRESOURCE_TYPE type = pResource->Type();

	if( type == RESOURCE_GEOMETRY )			{	g_MemPoolGeometry.Remove(pResource);	}
	else if( type == RESOURCE_TEXTURE )		{	g_MemPoolTexture.Remove(pResource);	}
	else if( type == RESOURCE_MESH )		{	g_MemPoolMesh.Remove(pResource);	}
	else if( type == RESOURCE_ACTOR )		{	g_MemPoolActor.Remove(pResource);	}
	else if( type == RESOURCE_MOTION )		{	g_MemPoolMotion.Remove(pResource);	}
	else if( type == RESOURCE_MATERIAL )	{	g_MemPoolMtrl.Remove(pResource);	}
	else
		assert(0);
}



//--------------------------------------------------------------------------------------------------------
EAssetMgr::EAssetMgr()
{
}

EAssetMgr::~EAssetMgr()
{

}

//-----------------------------------------------------------------------------------------------------------------------
CResourceBase* EAssetMgr::UnSafeGetResource( eRESOURCE_TYPE type, long id )
{
	TYPE_RESOURCE_MAP::CPair* itr = m_ResourceMap[type].Lookup( id );
	if( itr != NULL)
		return itr->m_value;

	return NULL;
}

const CResourceBase* EAssetMgr::GetResource( eRESOURCE_FILE_TYPE type, const char* name )
{
	const CResourceBase* pResource = NULL;

	if( type == RESOURCE_FILE_TEXTURE )			pResource = GetResource(RESOURCE_TEXTURE, name );
	else if( type == RESOURCE_FILE_MESH )		pResource = GetResource(RESOURCE_MESH, name );
	else if( type == RESOURCE_FILE_ACTOR )		pResource = GetResource(RESOURCE_ACTOR, name );
	else if( type == RESOURCE_FILE_MOTION )		pResource = GetResource(RESOURCE_MOTION, name );
	else if( type == RESOURCE_FILE_MATERIAL)	pResource = GetResource(RESOURCE_MATERIAL, name );
	else
		assert(0);

	return pResource;
}

//-----------------------------------------------------------------------------------------------------------------------
const CResourceBase* EAssetMgr::GetResource( eRESOURCE_TYPE type, long id )
{
	TYPE_RESOURCE_MAP::CPair* itr = m_ResourceMap[type].Lookup( id );
	if( itr != NULL)
		return itr->m_value;

	return NULL;
}

//-----------------------------------------------------------------------------------------------------------------------
void EAssetMgr::Clear()
{
	for( int i = 0; i < NUM_RESOURCE_TYPE; ++i )
	{
		if( i == RESOURCE_GEOMETRY )
			continue; // it will be deleted when mesh is deleted.

		POSITION pos = m_ResourceMap[i].GetStartPosition();
		TYPE_RESOURCE_MAP::CPair* itr = NULL;

		while (pos)
		{
			itr = m_ResourceMap[i].GetNext(pos);
			Remove( eRESOURCE_TYPE(i), itr->m_key );
		}
	}
}


//-----------------------------------------------------------------------------------------------------------------------
void EAssetMgr::Remove(eRESOURCE_TYPE type, long id)
{
	IRDevice* pRDevice = GLOBAL::RDevice();

	CResourceBase* pResource = m_ResourceMap[type].Lookup( id )->m_value;

	if( type == RESOURCE_MESH )
	{
		CResourceMesh* pMesh = (CResourceMesh*)m_ResourceMap[type].Lookup( id )->m_value;
		for(UINT i=0; i< pMesh->goemetries.size(); ++i)
		{
			Remove( RESOURCE_GEOMETRY, pMesh->goemetries[i]->RID);
			pMesh->goemetries[i] = NULL;
		}
		pMesh->goemetries.clear();
	}
	else if( type == RESOURCE_TEXTURE || type == RESOURCE_GEOMETRY)
	{
		pRDevice->RemoveGraphicBuffer(pResource);
	}

	MemRemove( pResource );
	m_ResourceMap[type].RemoveKey( id);
}


//-----------------------------------------------------------------------------------------------------------------------
void EAssetMgr::Reload( eRESOURCE_TYPE type, long id )
{
	const CResourceBase* pResource = GetResource(type, id);
	char name[64];
	strcpy_s( name, pResource->name);

	Remove(type, id);

	if( type == RESOURCE_ACTOR ) GLOBAL::Loader()->Load(name, RESOURCE_FILE_ACTOR, true );
	else if( type == RESOURCE_MESH ) GLOBAL::Loader()->Load(name, RESOURCE_FILE_MESH, true );
	else if( type == RESOURCE_MOTION ) GLOBAL::Loader()->Load(name, RESOURCE_FILE_MOTION, true );
	else if( type == RESOURCE_TEXTURE ) GLOBAL::Loader()->Load(name, RESOURCE_FILE_TEXTURE, true );
	else if( type == RESOURCE_MATERIAL ) GLOBAL::Loader()->Load(name, RESOURCE_FILE_MATERIAL, true );
}


//-----------------------------------------------------------------------------------------------------------------------
CResourceBase* EAssetMgr::CreateResource( eRESOURCE_TYPE type, const char* name)
{
	if( GetResource(type , name) != NULL )
	{
		assert(0);
		return NULL;
	}

	CResourceBase* pResource = MemGetNew(type);

	pResource->loadState = RESOURCE_MEMORY_CREATED;
	pResource->RID = GET_HASH_KEY( name );
	strcpy_s( pResource->name, name);
	m_ResourceMap[type].SetAt( pResource->RID, pResource );

	return pResource;
}
