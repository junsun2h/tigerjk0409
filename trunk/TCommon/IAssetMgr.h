#pragma once

#include "IResource.h"



typedef	ATL::CAtlMap<long, CResourceBase*>	TYPE_RESOURCE_MAP;

enum DEFFERED_RENDER_TARGET
{
	RENDER_TARGET_GEOMERTY,
	RENDER_TARGET_LIGHT,

	NUM_DEFFERED_RENDER_TARGET
};

struct IAssetMgr
{
	virtual long				Insert( CResourceBase* pResource) = 0;

	virtual void				Clear() = 0;
	virtual void				Remove(RESOURCE_TYPE type, long id) = 0;
	virtual void				Remove(RESOURCE_TYPE type, std::string& name) = 0;
	virtual void				Remove(RESOURCE_TYPE type, const char* name) = 0;

	virtual const CResourceBase*		GetResource( RESOURCE_TYPE type, long id ) = 0;
	virtual const CResourceBase*		GetResource( RESOURCE_TYPE type, const char* name ) = 0;
	virtual const CResourceBase*		GetResource( RESOURCE_TYPE type, std::string name ) = 0;
	virtual const TYPE_RESOURCE_MAP*	GetResources( RESOURCE_TYPE type ) = 0;
};

