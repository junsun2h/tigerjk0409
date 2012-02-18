#pragma once

#include "CMathType.h"

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
	OPAQUE_PASS,
	TRANSPARENT_PASS,

	NUM_RENDER_PASS
};


struct IRenderStrategy
{
	virtual	void	RenderFrame(CCAMERA_DESC* pCameraDesc) =0;
	virtual void	RenderGeometry(CResourceGeometry* pGeometry) =0;

	virtual void	SetMaterial(const CResourceMtrl* pMaterial, const CResourceGeometry* pGeometry) = 0;

	virtual	void	SetTransform( const XMMATRIX& worldTM ) = 0;
	virtual void	SetJointTransforms( XMMATRIX* pJointTM, UINT size ) = 0;
	
	virtual eRENDER_PASS	GetCurrentPass() = 0;
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

	virtual bool				StartUp(const CENGINE_INIT_PARAM* pRaram, IEngine* pEngine) = 0;
	virtual void				ShutDown() = 0;

	virtual void				Present() = 0;

	virtual bool				Resize(int width, int height) = 0;

	virtual	IRenderStrategy*	GetRenderer() = 0;
	virtual IRenderHelper*		GetRenderHelper() = 0;
	virtual	RDeviceDesc			GetDeviceSetting() = 0;

	// Device Dependent Resources ( ex: Texture, VB, IB, Shader )
	virtual void				CreateGraphicBuffer(CResourceBase* pResource) = 0;
	virtual void				RemoveGraphicBuffer(CResourceBase* pResource) =0;

	virtual CResourceTexture*	CreateTextureFromFile(const char* fileName, CResourceTexture* pTexture) = 0;
	virtual bool				SaveTextureToFile(const CResourceTexture* pTexture, eIMAGE_FILE_FORMAT format, const char* fileName) = 0;

	virtual HWND				GetHWND() = 0;
};
