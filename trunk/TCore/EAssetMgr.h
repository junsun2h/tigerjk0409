#pragma once


#include "CDefine.h"
#include "IAssetMgr.h"
#include "IEngine.h"


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
public:
	EAssetMgr();
	~EAssetMgr();

	void						Init(const CENGINE_INIT_PARAM &param);
	void						ResizeDefferedRenderTarget(UINT width, UINT height);

public:
	long						Insert( CResourceBase* pResource) override;

	const CResourceBase*		GetResource( RESOURCE_TYPE type, long id ) override;
	const CResourceBase*		GetResource( RESOURCE_TYPE type, const char* name ) override;
	const CResourceBase*		GetResource( RESOURCE_TYPE type, std::string name ) override;
	const TYPE_RESOURCE_MAP*	GetResources( RESOURCE_TYPE type ) override			{ return &m_Resources[type]; }

	void						Clear() override;
	void						Remove(RESOURCE_TYPE type, long id) override;
	void						Remove(RESOURCE_TYPE type, std::string& name) override;
	void						Remove(RESOURCE_TYPE type, const char* name) override;

private:
	void						CreateDefferedRenderTarget(UINT width, UINT height);
	void						GenerateMapData(const wchar_t* path);

	TYPE_RESOURCE_MAP			m_Resources[NUM_RESOURCE_TYPE];
	CResourceTexture*			m_DefferdRenderTargets[NUM_DEFFERED_RENDER_TARGET];
};