#pragma once

#include "CDefine.h"
#include "IRDevice.h"
#include "IActorMgr.h"
#include "IEntityMgr.h"
#include "IAssetMgr.h"
#include "ISceneMgr.h"
#include "IRenderCallback.h"
#include "CTimer.h"
#include "CText.h"
#include "IFileUtility.h"
#include "ILoader.h"


struct CENGINE_INIT_PARAM
{
	CENGINE_INIT_PARAM()
		: hWnd(NULL)
		, width(-1)
		, height(-1)
		, bFullScreen(false)
		, numOfProcessThread(1)
		, resourceFolder(NULL)
	{
	}

	void*			hWnd;
	int				width;
	int				height;
	bool			bFullScreen;
	UINT			numOfProcessThread;
	const wchar_t*	resourceFolder;
};



struct IEngine
{
	virtual bool			StartUp(const CENGINE_INIT_PARAM &param) = 0;
	virtual bool			ShutDown() = 0;
	virtual void			Resize(UINT width, UINT height) = 0;

	virtual IRDevice*		RDevice() = 0;
	virtual IAssetMgr*		AssetMgr() = 0;
	virtual ISceneMgr*		SceneMgr() = 0;
	virtual IEntityMgr*		EntityMgr() =0;
	virtual IRenderHelper*	RenderHelper() =0;
	virtual CTimer*			GlobalTimer() = 0;
	virtual IFileUtility*	FileUtility() = 0;
	virtual ILoader*		Loader() = 0;

	virtual void			UpdateAndRender(IEntityProxyCamera* pCamera, IRenderingCallback* pRenderCallback) = 0;
};