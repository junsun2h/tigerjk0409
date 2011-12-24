#pragma once

#include <string>


struct IEntity;
enum ENTITY_EVENT;


enum ENTITY_PROXY_TYPE
{
	ENTITY_STATIC_MESH,
	ENTITY_ACTOR_INSTANCE,
	ENTITY_EFFECT,

	NUM_ENTITY_PROXY_TYPE
};


struct CEntityProxyCreateParam
{
	ENTITY_PROXY_TYPE	type;
	std::string			name;
	std::string			sourceName;	// data name like effect, actor, mesh ...
};


struct IEntityProxy
{
	virtual ~IEntityProxy(){}

	virtual ENTITY_PROXY_TYPE GetType() = 0;
	virtual bool Init( IEntity *pEntity ) = 0;
	virtual void Release() = 0;
	virtual	void Update() = 0;
	virtual	void ProcessEvent( ENTITY_EVENT &event ) = 0;
};

