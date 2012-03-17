#pragma once

#include "CLight.h"

typedef std::vector<CLightDesc*> LIGHT_LIST;


struct ILightMgr
{
	virtual	void				Destroy() = 0;
	virtual CLightDesc*			Create() = 0;
	virtual CLightDesc*			Find(UINT handle) = 0;
	virtual void				Remove(UINT handle) = 0;
	virtual	void				UpdateVisible() = 0;
	virtual const LIGHT_LIST*	GetVisibleLights() = 0;
};

