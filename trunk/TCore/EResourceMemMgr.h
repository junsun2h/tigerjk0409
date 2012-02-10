#pragma once


class EResourceMemMgr : public IResourceMemMgr
{
public:
	EResourceMemMgr();
	~EResourceMemMgr();

	CResourceBase*			GetNew(eRESOURCE_TYPE type) override;
	void					Remove(CResourceBase* pResource) override;

private:
	CObjectPool<CResourceGeometry>	m_MemPoolGeometry;
	CObjectPool<CResourceMesh>		m_MemPoolMesh;
	CObjectPool<CResourceTexture>	m_MemPoolTexture;
	CObjectPool<CResourceActor>		m_MemPoolActor;
	CObjectPool<CResourceMotion>	m_MemPoolMotion;
	CObjectPool<CResourceMtrl>		m_MemPoolMtrl;
};
