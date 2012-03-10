#pragma once

#include "CLight.h"

struct ILightMgr
{
	virtual	void		Destroy() = 0;
	virtual CLightDesc*	Create() = 0;
	virtual CLightDesc*	Find(UINT handle) = 0;
	virtual void		Remove(UINT handle) = 0;
	virtual void		GetAffectLight(IEntity* pEntity, LIGHT_LIST* pLightList) = 0;
};

