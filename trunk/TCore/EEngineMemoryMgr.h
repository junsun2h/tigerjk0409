#pragma once

#include "IEngineMemoryMgr.h"
#include "CUnitPool.h"
#include "EEntity.h"
#include "EEntityProxyActor.h"
#include "EEntityProxyCamera.h"
#include "EEntityProxyRender.h"



class EEngineMemoryMgr : public IEngineMemoryMgr
{
public:
	EEngineMemoryMgr();
	~EEngineMemoryMgr();

	virtual CResourceBase*	GetNewResource(eRESOURCE_TYPE type) override;

public:
	void					RemoveResource(CResourceBase* pResource);

	EEntity*				GetNewEntity();
	void					RemoveEntity(IEntity* pEntity);

	IEntityProxy*			GetNewProxy(eENTITY_PROXY_TYPE type);
	void					RemoveProxy(IEntityProxy* pProxy);

private:
	CObjectPool<CResourceGeometry>	m_MemPoolGeometry;
	CObjectPool<CResourceMesh>		m_MemPoolMesh;
	CObjectPool<CResourceTexture>	m_MemPoolTexture;
	CObjectPool<CResourceActor>		m_MemPoolActor;
	CObjectPool<CResourceMotion>	m_MemPoolMotion;
	CObjectPool<CResourceMtrl>		m_MemPoolMtrl;

	CObjectPool<EEntity>			m_MemPoolEntity;

	CObjectPool<EEntityProxyActor>	m_MemPoolProxyActor;
	CObjectPool<EEntityProxyCamera>	m_MemPoolProxyCamera;
	CObjectPool<EEntityProxyRender>	m_MemPoolProxyRender;
};
