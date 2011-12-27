#pragma once

#include <string>
#include "IResource.h"
#include "IMotionMgr.h"

struct IEntity;
struct EntityEvent;
enum ENTITY_TYPE;


enum ENTITY_PROXY_TYPE
{
	ENTITY_STATIC_MESH,
	ENTITY_ACTOR,
	ENTITY_EFFECT,

	NUM_ENTITY_PROXY_TYPE
};


struct IEntityProxy
{
	virtual ~IEntityProxy(){}

	virtual ENTITY_PROXY_TYPE	GetType() = 0;
	virtual long				GetID() = 0;
	virtual long				GetEntityID() = 0;
	virtual void				SetEntity(IEntity* pEntity) = 0;

	virtual	void				ProcessEvent( EntityEvent &event ) = 0;
};

struct IEntityProxyActor : IEntityProxy
{
	virtual void					Init(std::string strResource) = 0;
	virtual ENTITY_PROXY_TYPE		GetType()	{ return ENTITY_ACTOR; };
	virtual const CResourceActor*	GetActorInfo() = 0;
	virtual IMotionMgr*				GetMotionMgr() = 0;
	virtual void					Update(float deltaTime) = 0;
};