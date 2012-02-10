#pragma once


class EEntityMgr : public IEntityMgr
{
public:
	EEntityMgr();

	IEntity*			SpawnEntity(std::string name) override;

	IEntity*			GetEntity(std::string name) override	{ return GetEntity( GET_HASH_KEY(name) );}
	IEntity*			GetEntity(long id) override;

	void				RemoveEntity(long id) override;
	void				RemoveEntity(const char* name) override	{ return RemoveEntity( GET_HASH_KEY(name) ); }

	void				RemoveAllEntity() override;
	void				Destroy() override;

	void				SaveXML(IEntity* pEntity);

private:
	typedef ATL::CAtlMap<long, IEntity*>	ENTITY_MAP;
	
	ENTITY_MAP				m_EntityMap;
	CObjectPool<EEntity>	m_MemPoolEntity;
};
