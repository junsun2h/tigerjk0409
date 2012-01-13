#pragma once


#include "CDefine.h"
#include "IAsyncLoader.h"
#include "IResource.h"
#include "EAsyncLoader.h"
#include <atlcoll.h>

struct IRDevice;

enum HELPER_GEOMETRY_TYPE
{
	HELPER_BOX,
	HELPER_SPHERE,
	HELPER_AXIS,
	HELPER_GRID
};

struct CHelperGeometryCreateParam
{
	HELPER_GEOMETRY_TYPE	type;
	float					radius;
	float					size;
};

class EAssetMgr : public IAssetMgr
{
	IRDevice*				m_pRDevice;
	EAsyncLoader			m_AsyncLoader;

	TYPE_RESOURCE_MAP		m_Resources[NUM_RESOURCE_TYPE];

public:
	EAssetMgr();
	~EAssetMgr();

	void						Init( UINT numProcessThread, IRDevice*	pRDevice );

public:
	long						LoadCompletedResource( CResourceBase* pResource) override;
	long						Load(char* fileName, RESOURCE_FILE_TYPE type, CALLBACK_LOAD_COMPLED pCallback = NULL, bool bAsync = true) override;

	const CResourceBase*		GetResource( RESOURCE_TYPE type, long id ) override;
	const CResourceBase*		GetResource( RESOURCE_TYPE type, std::string name ) override;
	const TYPE_RESOURCE_MAP*	GetResources( RESOURCE_TYPE type ) override			{ return &m_Resources[type]; }

	void						Clear() override;
	void						Remove(RESOURCE_TYPE type, long id) override;
	void						Remove(RESOURCE_TYPE type, std::string& name) override;
};