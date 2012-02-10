#pragma once


#include "CResource.h"


struct IResourceMemMgr
{
	virtual CResourceBase*	GetNew(eRESOURCE_TYPE type) = 0;
	virtual void			Remove(CResourceBase* pResource) = 0;
};