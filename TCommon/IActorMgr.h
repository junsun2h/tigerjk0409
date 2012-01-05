#pragma once


#include "IEntityProxy.h"


struct IActorMgr
{
	virtual void				Clear() = 0;
	virtual IEntityProxyActor*	SPawn(std::string name) = 0;
	virtual void				Update(float deltaTime) = 0;
};

struct ICameraMgr
{
	virtual void				Clear() = 0;
	virtual IEntityProxyCamera*	SPawn(std::string name) = 0;
};