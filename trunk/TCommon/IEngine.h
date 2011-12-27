#pragma once

#include "CDefine.h"
#include "IRDevice.h"
#include "IActorMgr.h"
#include "IEntityMgr.h"


struct CENGINE_INIT_PARAM
{
	CENGINE_INIT_PARAM()
		: hInstance(NULL)
		, hWnd(NULL)
		, width(-1)
		, height(-1)
		, bFullScreen(false)
		, numOfProcessThread(1)
	{
	}

	void *hInstance;
	void *hWnd;
	int width;
	int height;
	bool bFullScreen;
	UINT numOfProcessThread;
};



struct IEngine
{
	virtual bool		StartUp(const CENGINE_INIT_PARAM &param) = 0;
	virtual bool		ShutDown() = 0;

	virtual IRDevice*	RDevice() = 0;
	virtual IEntityMgr* EntityMgr() = 0;
	virtual IActorMgr*	AcotrMgr() = 0;
	virtual IAssetMgr*	AssetMgr() = 0;
};