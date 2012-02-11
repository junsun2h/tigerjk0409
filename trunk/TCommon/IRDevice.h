#pragma once

struct IEngine;
struct IRenderHelper;
struct IEntityProxyRender;
struct CResourceBase;
struct CCAMERA_DESC;
struct CENGINE_INIT_PARAM;

class CResourceTexture;
class CResourceGeometry;
class CResourceMtrl;

enum eIMAGE_FILE_FORMAT;


enum eRENDER_COMMAND
{
	RC_RESET_DEVICE,
};

enum eRENDER_STRATEGY
{
	RS_DEFFERED
};

enum eRENDER_PASS
{
	R_SKIN_PASS,
	R_GEOMETRY_PASS,
	R_LIGHT_PASS,
	R_MATERIAL_PASS,

	R_SHADOW_PASS_00,
	R_SHADOW_PASS_01,
	R_SHADOW_PASS_02,
	R_SHADOW_PASS_03,
	R_SHADOW_PASS_04,
	R_SHADOW_PASS_05,

	NUM_RENDER_PASS
};


struct IRenderStrategy
{
	virtual void RenderScene() = 0;
};

struct RDeviceDesc
{
	int					width;
	int					height;
};


struct IRenderingCallback
{
	virtual void PreRender(){}
	virtual void PostRender(){}
};


struct IRDevice  
{
	virtual ~IRDevice(){}

	virtual bool			StartUp(const CENGINE_INIT_PARAM* pRaram, IEngine* pEngine) = 0;
	virtual void			ShutDown() = 0;

	virtual void			RenderFrame(CCAMERA_DESC* pCameraDesc) = 0;
	virtual void			RenderElement( CResourceGeometry*	pGeometry, CResourceMtrl* pMtrl, IEntityProxyRender* pRenderProxy) = 0;
	virtual void			Present() = 0;

	virtual bool			Resize(int width, int height) = 0;

	virtual IRenderHelper*	GetRenderHelper() = 0;
	virtual	RDeviceDesc		GetDeviceSetting() = 0;

	// Device Dependent Resources ( ex: Texture, VB, IB, Shader )
	virtual void			PT_CreateGraphicBuffer(CResourceBase* pResource) = 0;
	virtual void			CreateGraphicBuffer(CResourceBase* pResource) = 0;
	virtual void			RemoveGraphicBuffer(CResourceBase* pResource) =0;

	virtual CResourceTexture*	CreateTextureFromFile(const char* fileName) = 0;
	virtual bool				SaveTextureToFile(const CResourceTexture* pTexture, eIMAGE_FILE_FORMAT format, const char* fileName) = 0;

	virtual HWND			GetHWND() = 0;

	virtual eRENDER_PASS	GetCurrentPass() = 0;
};
