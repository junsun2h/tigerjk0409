#pragma once

#include <atlcoll.h>
#include <string>

enum eRESOURCE_TYPE;
struct CResourceBase;

typedef	ATL::CAtlMap<long, CResourceBase*>	TYPE_RESOURCE_MAP;

struct IAssetMgr
{
	virtual long				Insert( CResourceBase* pResource) = 0;

	virtual void				Clear() = 0;
	virtual void				Remove(eRESOURCE_TYPE type, long id) = 0;
	virtual void				Remove(eRESOURCE_TYPE type, std::string& name) = 0;
	virtual void				Remove(eRESOURCE_TYPE type, const char* name) = 0;

	virtual CResourceBase*		UnSafeGetResource( eRESOURCE_TYPE type, long id ) = 0;
	virtual CResourceBase*		UnSafeGetResource( eRESOURCE_TYPE type, const char* name ) = 0;

	virtual const CResourceBase*		GetResource( eRESOURCE_TYPE type, long id ) = 0;
	virtual const CResourceBase*		GetResource( eRESOURCE_TYPE type, const char* name ) = 0;
	virtual const CResourceBase*		GetResource( eRESOURCE_TYPE type, std::string name ) = 0;
	virtual const TYPE_RESOURCE_MAP*	GetResources( eRESOURCE_TYPE type ) = 0;
};

