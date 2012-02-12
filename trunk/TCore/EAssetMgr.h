#pragma once

enum eHELPER_GEOMETRY_TYPE
{
	HELPER_BOX,
	HELPER_SPHERE,
	HELPER_AXIS,
	HELPER_GRID
};

struct CHelperGeometryCreateParam
{
	eHELPER_GEOMETRY_TYPE	type;
	float					radius;
	float					size;
};

class EAssetMgr : public IAssetMgr
{
public:
	EAssetMgr();
	~EAssetMgr();

public:
	CResourceBase*				UnSafeGetResource( eRESOURCE_TYPE type, long id ) override;
	CResourceBase*				UnSafeGetResource( eRESOURCE_TYPE type, const char* name ) override { 	return UnSafeGetResource(type, GET_HASH_KEY(name) ); }

	CResourceBase*				CreateResource( eRESOURCE_TYPE type, const char* name) override;

	const CResourceBase*		GetResource( eRESOURCE_TYPE type, long id ) override;
	const CResourceBase*		GetResource( eRESOURCE_TYPE type, const char* name ) override	{ return GetResource(type, GET_HASH_KEY(name) ); }
	const CResourceBase*		GetResource( eRESOURCE_TYPE type, std::string name ) override	{ return GetResource(type, GET_HASH_KEY(name) ); }
	const TYPE_RESOURCE_MAP*	GetResources( eRESOURCE_TYPE type ) override					{ return &m_ResourceMap[type]; }

	void						Reload( eRESOURCE_TYPE type, long id );

	void						Clear() override;

	void						Remove(eRESOURCE_TYPE type, long id) override;
	void						Remove(eRESOURCE_TYPE type, std::string& name) override	{	Remove(type, GET_HASH_KEY(name));	}
	void						Remove(eRESOURCE_TYPE type, const char* name) override	{	Remove(type, GET_HASH_KEY(name));	}

private:
	void						CreateDefferedRenderTarget(UINT width, UINT height);
	void						GenerateMapData(const wchar_t* path);

	TYPE_RESOURCE_MAP			m_ResourceMap[NUM_RESOURCE_TYPE];
};