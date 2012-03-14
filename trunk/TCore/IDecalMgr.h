#pragma once


#include "CDecal.h"


struct IDecalMgr
{
	virtual void			Destroy() = 0;
	virtual CDecalDesc*		Create() = 0;
	virtual CDecalDesc*		Find(UINT handle) = 0;
	virtual void			Remove(UINT handle) = 0;
	virtual void			UpdateVisible() = 0;
};
