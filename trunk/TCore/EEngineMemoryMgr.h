#pragma once


class EEngineMemoryMgr : public IEngineMemoryMgr
{
public:
	EEngineMemoryMgr();
	~EEngineMemoryMgr();

	CResourceBase*			GetNewResource(eRESOURCE_TYPE type) override;
	void					RemoveResource(CResourceBase* pResource) override;

	IEntity*				GetNewEntity() override;
	void					RemoveEntity(IEntity* pEntity) override;

	IEntityProxy*			GetNewProxy(eENTITY_PROXY_TYPE type) override;
	void					RemoveProxy(IEntityProxy* pProxy) override;

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
