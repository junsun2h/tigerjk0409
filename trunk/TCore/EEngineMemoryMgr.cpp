#include <atlcoll.h>

#include "IRDevice.h"
#include "IEngineMemoryMgr.h"

#include "CUnitPool.h"
#include "CGrowableArray.h"

#include "EAABB.h"
#include "EEntity.h"
#include "EEntityProxyActor.h"
#include "EEntityProxyCamera.h"
#include "EEntityProxyRender.h"
#include "EEngineMemoryMgr.h"



EEngineMemoryMgr::EEngineMemoryMgr()
	: m_MemPoolGeometry(100)
	, m_MemPoolMesh(100)
	, m_MemPoolTexture(100)
	, m_MemPoolActor(100)
	, m_MemPoolMotion(100)
	, m_MemPoolMtrl(100)
	, m_MemPoolEntity(100)
	, m_MemPoolProxyActor(100)
	, m_MemPoolProxyCamera(100)
	, m_MemPoolProxyRender(100)
{
}

EEngineMemoryMgr::~EEngineMemoryMgr()
{
}

CResourceBase*	EEngineMemoryMgr::GetNewResource(eRESOURCE_TYPE type)
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

void EEngineMemoryMgr::RemoveResource(CResourceBase* pResource)
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

IEntity* EEngineMemoryMgr::GetNewEntity() 
{
	return m_MemPoolEntity.GetNew();
}

void EEngineMemoryMgr::RemoveEntity(IEntity* pEntity)
{
	return m_MemPoolEntity.Remove(pEntity);
}

IEntityProxy* EEngineMemoryMgr::GetNewProxy(eENTITY_PROXY_TYPE type)
{
	if( type == ENTITY_PROXY_ACTOR )			{	return m_MemPoolProxyActor.GetNew();	}
	else if( type == ENTITY_PROXY_RENDER )		{	return m_MemPoolProxyRender.GetNew();	}
	else if( type == ENTITY_PROXY_CAMERA )		{	return m_MemPoolProxyCamera.GetNew();	}

	assert(0);
	return NULL;
}

void EEngineMemoryMgr::RemoveProxy(IEntityProxy* pProxy)
{
	eENTITY_PROXY_TYPE type = pProxy->GetType();

	if( type == ENTITY_PROXY_ACTOR )			{	return m_MemPoolProxyActor.Remove(pProxy);	}
	else if( type == ENTITY_PROXY_RENDER )		{	return m_MemPoolProxyRender.Remove(pProxy);	}
	else if( type == ENTITY_PROXY_CAMERA )		{	return m_MemPoolProxyCamera.Remove(pProxy);	}
	else
		assert(0);
}
