#pragma once

enum eRESOURCE_TYPE;
struct CResourceBase;

struct IResourceMemMgr
{
	virtual CResourceBase*	GetNew(eRESOURCE_TYPE type) = 0;
	virtual void			Remove(CResourceBase* pResource) = 0;
};