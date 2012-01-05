#pragma once

#include "IResource.h"

struct CENGINE_INIT_PARAM;



enum RENDER_COMMAND
{
	RC_RESET_DEVICE,
};


struct IRDevice  
{
	virtual ~IRDevice(){}

	virtual bool		StartUp(const CENGINE_INIT_PARAM &param) = 0;
	virtual void		ShutDown() = 0;

	virtual void		Render(uint32 index) = 0;
	virtual	void		RenderUI() = 0;
	virtual void		RenderLines() = 0;
	virtual void		Present() = 0;

	virtual bool		Resize(int width, int height) = 0;

	// Device Dependent Resources ( ex: Texture, VB, IB, Shader )
	virtual void		TS_CreateDPResource(DEVICE_DEPENDENT_RESOURCE type, void* pBuf ,int size, IResource* pResource) = 0;
	virtual HWND		GetHWND() = 0;
};
