#pragma once

#include <atlcoll.h>
#include <string>

enum eRESOURCE_TYPE;
enum eRESOURCE_FILE_TYPE;
struct CResourceBase;

typedef	ATL::CAtlMap<long, CResourceBase*>	TYPE_RESOURCE_MAP;

struct IAssetMgr
{
	virtual void				Clear() = 0;

	virtual void				Remove(const CResourceBase* pResource) = 0;
	virtual void				Remove(eRESOURCE_TYPE type, long id) = 0;
	virtual void				Remove(eRESOURCE_TYPE type, std::string& name) = 0;
	virtual void				Remove(eRESOURCE_TYPE type, const char* name) = 0;

	virtual CResourceBase*		UnSafeGetResource( eRESOURCE_TYPE type, long id ) = 0;
	virtual CResourceBase*		UnSafeGetResource( eRESOURCE_TYPE type, const char* name ) = 0;

	virtual	CResourceBase*		CreateResource( eRESOURCE_TYPE type, const char* name) = 0;

	virtual void				Reload( eRESOURCE_TYPE type, long id ) = 0;

	virtual const CResourceBase*		GetResource( eRESOURCE_FILE_TYPE type, const char* name ) =0;
	virtual const CResourceBase*		GetResource( eRESOURCE_TYPE type, long id ) = 0;
	virtual const CResourceBase*		GetResource( eRESOURCE_TYPE type, const char* name ) = 0;
	virtual const CResourceBase*		GetResource( eRESOURCE_TYPE type, std::string name ) = 0;
	virtual const TYPE_RESOURCE_MAP*	GetResourceMap( eRESOURCE_TYPE type ) = 0;
};

