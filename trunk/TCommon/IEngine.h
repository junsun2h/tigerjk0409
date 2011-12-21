#pragma once

#include "CDefine.h"
#include "IRDevice.h"

struct IRDevice;

struct CENGINE_INIT_PARAM
{
	CENGINE_INIT_PARAM()
		: hInstance(NULL)
		, hWnd(NULL)
		, width(-1)
		, height(-1)
		, bFullScreen(false)
	{
	}

	void *hInstance;
	void *hWnd;
	int width;
	int height;
	bool bFullScreen;
};



struct IEngine
{
	virtual ~IEngine(){}

	virtual bool		StartUp(const CENGINE_INIT_PARAM &param) = 0;
	virtual bool		ShutDown() = 0;

	virtual	IRDevice*	GetRenderer() = 0;
};