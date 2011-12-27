#pragma once


#include "CDefine.h"
#include "IAsyncLoader.h"
#include "IResource.h"
#include "EAsyncLoader.h"
#include <atlcoll.h>

struct IRDevice;



class EAssetMgr : public IAssetMgr
{
	IRDevice*				m_pRDevice;
	EAsyncLoader			m_AsyncLoader;

	typedef	ATL::CAtlMap<long, IResource*>	TYPE_RESOURCE_MAP;
	TYPE_RESOURCE_MAP		m_Resources[NUM_RESOURCE_TYPE];

public:
	EAssetMgr();
	~EAssetMgr();

	void				Init( UINT numProcessThread, IRDevice*	pRDevice );

	void				LoadCompletedResource( IResource* pResource);
	long				Load(char* fileName, RESOURCE_FILE_TYPE type, CALLBACK_LOAD_COMPLED pCallback = NULL, bool bAsync = true);

	const IResource*	GetResource( RESOURCE_TYPE type, long id );
	const IResource*	GetResource( RESOURCE_TYPE type, std::string name );

	void				Clear() override;
};