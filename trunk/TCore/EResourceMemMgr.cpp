#include <atlcoll.h>

#include "CResource.h"
#include "CUnitPool.h"
#include "CGrowableArray.h"

#include "IResourceMemMgr.h"

#include "EResourceMemMgr.h"



EResourceMemMgr::EResourceMemMgr()
	: m_MemPoolGeometry(100)
	, m_MemPoolMesh(100)
	, m_MemPoolTexture(100)
	, m_MemPoolActor(100)
	, m_MemPoolMotion(100)
	, m_MemPoolMtrl(100)
{
}

EResourceMemMgr::~EResourceMemMgr()
{
}

CResourceBase*	EResourceMemMgr::GetNew(eRESOURCE_TYPE type)
{
	if( type == RESOURCE_GEOMETRY )			{	return m_MemPoolGeometry.GetNew();	}
	else if( type == RESOURCE_TEXTURE )		
	{
		CResourceTexture* pTexture = m_MemPoolTexture.GetNew();
		pTexture->Init();
		return pTexture;	
	}
	else if( type == RESOURCE_MESH )		{	return m_MemPoolMesh.GetNew();	}
	else if( type == RESOURCE_ACTOR )		{	return m_MemPoolActor.GetNew();	}
	else if( type == RESOURCE_MOTION )		{	return m_MemPoolMotion.GetNew();	}
	else if( type == RESOURCE_MATERIAL )	{	return m_MemPoolMtrl.GetNew();	}

	assert(0);
	return NULL;
}

void EResourceMemMgr::Remove(CResourceBase* pResource)
{
	eRESOURCE_TYPE type = pResource->Type();

	if( type == RESOURCE_GEOMETRY )			{	m_MemPoolGeometry.Remove(pResource);	}
	else if( type == RESOURCE_TEXTURE )		{	m_MemPoolTexture.Remove(pResource);	}
	else if( type == RESOURCE_MESH )		{	m_MemPoolMesh.Remove(pResource);	}
	else if( type == RESOURCE_ACTOR )		{	m_MemPoolActor.Remove(pResource);	}
	else if( type == RESOURCE_MOTION )		{	m_MemPoolMotion.Remove(pResource);	}
	else if( type == RESOURCE_MATERIAL )	{	m_MemPoolMtrl.Remove(pResource);	}
	else
		assert(0);
}