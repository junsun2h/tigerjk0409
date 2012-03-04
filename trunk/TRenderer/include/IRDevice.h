#pragma once

#include "CMathType.h"

struct IRenderHelper;
struct IEntityProxyRender;
struct CResourceBase;
struct CCAMERA_DESC;
struct CENGINE_INIT_PARAM;
struct IShaderMgr;
struct CRenderElement;

class CResourceTexture;
class CResourceGeometry;
class CResourceMtrl;

enum eIMAGE_FILE_FORMAT;


enum eRENDER_STRATEGY
{
	RENDER_STRATEGY_FORWARD,
	RENDER_STRATEGY_DEFFERED
};

enum eRENDER_PASS
{
	RENDER_PASS_OPAQUE,
	RENDER_PASS_TRANSPARENT,

	NUM_RENDER_PASS
};


struct IRenderStrategy
{
	virtual	void	RenderFrame(CCAMERA_DESC* pCameraDesc) =0;
	virtual void	RenderGeometry(CResourceGeometry* pGeometry) =0;
};

struct RDeviceDesc
{
	int					width;
	int					height;
};

struct IRDevice  
{
	virtual ~IRDevice(){}

	virtual bool				StartUp(const CENGINE_INIT_PARAM* pRaram) = 0;
	virtual void				ShutDown() = 0;

	virtual void				Present() = 0;

	virtual bool				Resize(int width, int height) = 0;

	virtual	IShaderMgr*			GetShaderMgr() = 0;
	virtual	IRenderStrategy*	GetRenderStrategy() = 0;
	virtual IRenderHelper*		GetRenderHelper() = 0;
	virtual	RDeviceDesc			GetDeviceSetting() = 0;

	// Device Dependent Resources ( ex: Texture, VB, IB, Shader )
	virtual void				CreateGraphicBuffer(CResourceBase* pResource) = 0;
	virtual void				RemoveGraphicBuffer(CResourceBase* pResource) =0;

	virtual void				CreateTextureFromMemory(BYTE* pData, size_t size, CResourceTexture* pTexture) = 0;
	virtual void				CreateTextureFromFile(const char* fileName, CResourceTexture* pTexture) = 0;
	virtual bool				SaveTextureToFile(const CResourceTexture* pTexture, eIMAGE_FILE_FORMAT format, const char* fileName) = 0;

	virtual HWND				GetHWND() = 0;
};
