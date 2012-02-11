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
	long						Insert( CResourceBase* pResource) override;

	CResourceBase*				UnSafeGetResource( eRESOURCE_TYPE type, long id ) override;
	CResourceBase*				UnSafeGetResource( eRESOURCE_TYPE type, const char* name ) override;
	const CResourceBase*		GetResource( eRESOURCE_TYPE type, long id ) override;
	const CResourceBase*		GetResource( eRESOURCE_TYPE type, const char* name ) override;
	const CResourceBase*		GetResource( eRESOURCE_TYPE type, std::string name ) override;
	const TYPE_RESOURCE_MAP*	GetResources( eRESOURCE_TYPE type ) override			{ return &m_Resources[type]; }

	void						Clear() override;
	void						Remove(eRESOURCE_TYPE type, long id) override;
	void						Remove(eRESOURCE_TYPE type, std::string& name) override;
	void						Remove(eRESOURCE_TYPE type, const char* name) override;

private:
	void						CreateDefferedRenderTarget(UINT width, UINT height);
	void						GenerateMapData(const wchar_t* path);

	TYPE_RESOURCE_MAP			m_Resources[NUM_RESOURCE_TYPE];
};