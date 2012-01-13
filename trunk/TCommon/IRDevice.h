#pragma once

#include "IResource.h"
#include "IRenderHelper.h"


struct CENGINE_INIT_PARAM;
struct CCAMERA_DESC;

enum RENDER_COMMAND
{
	RC_RESET_DEVICE,
};


struct IRDevice  
{
	virtual ~IRDevice(){}

	virtual bool			StartUp(const CENGINE_INIT_PARAM &param) = 0;
	virtual void			ShutDown() = 0;

	virtual void			Render(const CCAMERA_DESC& cameraDesc) = 0;
	virtual void			Present() = 0;

	virtual bool			Resize(int width, int height) = 0;

	virtual IRenderHelper*	GetRenderHelper() = 0;

	// Device Dependent Resources ( ex: Texture, VB, IB, Shader )
	virtual void			PT_CreateGraphicBuffer(CResourceBase* pResource) = 0;
	virtual void			CreateGraphicBuffer(CResourceBase* pResource) = 0;
	virtual void			RemoveGraphicBuffer(CResourceBase* pResource) =0;

	virtual HWND			GetHWND() = 0;
};
