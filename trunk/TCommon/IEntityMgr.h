#pragma once

struct IEntity;

struct IEntityMgr
{
	virtual IEntity*		SpawnEntity(std::string name) =0;

	virtual IEntity*		GetEntity(std::string name) =0;
	virtual IEntity*		GetEntity(long id) =0;

	virtual void			RemoveEntity(long id)  =0;
	virtual void			RemoveEntity(const char* name)  =0;

	virtual void			RemoveAllEntity()  =0;

	virtual	void			Destroy() = 0;
};
