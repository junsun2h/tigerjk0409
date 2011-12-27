#pragma once

#include "IEntity.h"

struct IEntityMgr
{
	virtual void		Clear() = 0;
	virtual	IEntity*	SPawn(std::string& name) = 0;
};