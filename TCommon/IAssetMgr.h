#pragma once

#include "CResource.h"
#include <atlcoll.h>


typedef	ATL::CAtlMap<long, CResourceBase*>	TYPE_RESOURCE_MAP;

enum eDEFFERED_RENDER_TARGET
{
	RENDER_TARGET_GEOMERTY,
	RENDER_TARGET_LIGHT,

	NUM_DEFFERED_RENDER_TARGET
};

struct IAssetMgr
{
	virtual long				Insert( CResourceBase* pResource) = 0;

	virtual void				Clear() = 0;
	virtual void				Remove(eRESOURCE_TYPE type, long id) = 0;
	virtual void				Remove(eRESOURCE_TYPE type, std::string& name) = 0;
	virtual void				Remove(eRESOURCE_TYPE type, const char* name) = 0;

	virtual const CResourceBase*		GetResource( eRESOURCE_TYPE type, long id ) = 0;
	virtual const CResourceBase*		GetResource( eRESOURCE_TYPE type, const char* name ) = 0;
	virtual const CResourceBase*		GetResource( eRESOURCE_TYPE type, std::string name ) = 0;
	virtual const TYPE_RESOURCE_MAP*	GetResources( eRESOURCE_TYPE type ) = 0;
};

