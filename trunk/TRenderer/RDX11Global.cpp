#include <atlcoll.h>

#include "CVertex.h"
#include "CResource.h"
#include "CGrowableArray.h"
#include "CCamera.h"
#include "CEngineParam.h"
#include "CQuad.h"

#include "RDefine.h"

#include "IRDevice.h"
#include "IShader.h"
#include "IFontRenderer.h"
#include "IRenderHelper.h"
#include "IRenderStateMgr.h"
#include "IRenderTargetMgr.h"

#include "RDX11RenderStateMgr.h"
#include "RDX11RenderTargetMgr.h"
#include "RDX11Device.h"
#include "RDX11FontRenderer.h"
#include "RDX11RenderHelper.h"
#include "RDX11ShaderMgr.h"
#include "RDX11Global.h"


namespace GLOBAL
{
	IEngine*					g_pEngine = NULL;

	ID3D11Device*				g_D3Device = NULL;
	ID3D11DeviceContext*		g_D3DeviceContext = NULL;

	RDeviceDesc					g_DeviceSetting;
	CCAMERA_DESC				g_pCurrentCameraDesc;

	RDX11Device					g_RDX11Device;
	RDX11RenderStateMgr			g_StateRepository;
	RDX11RenderTargetMgr		g_RenderTargetMgr;
	RDX11RenderHelper			g_RenderHelper;
	RDX11ShaderMgr				g_ShaderMgr;
	RDX11FontRenderer			g_FontRenderer;

	IEngine*					Engine()					{ return g_pEngine; }

	ID3D11Device*				D3DDevice()					{ return g_D3Device;}
	ID3D11DeviceContext*		D3DContext()				{ return g_D3DeviceContext; }

	const RDeviceDesc&			DeviceInfo()				{ return g_DeviceSetting; }
	const CCAMERA_DESC*			CameraDesc()				{ return &g_pCurrentCameraDesc; }
	void						SetCameraDesc(CCAMERA_DESC* pDesc)	{ g_pCurrentCameraDesc = *pDesc; }

	IRDevice*					RDevice()					{ return &g_RDX11Device; }
	IRenderStateMgr*			RenderStateMgr()			{ return &g_StateRepository; }
	IShaderMgr*					ShaderMgr()					{ return &g_ShaderMgr; }
	IFontRenderer*				FontRenderer()				{ return &g_FontRenderer; }
	IRenderTargetMgr*			RenderTargetMgr()			{ return &g_RenderTargetMgr; }
	IRenderHelper*				RenderHelper()				{ return &g_RenderHelper; }

	void Present()
	{
		g_RenderTargetMgr.Present();
	}


	//----------------------------------------------------------------------------------------------------------
	bool Resize(int width, int height)
	{
		g_DeviceSetting.width = width;
		g_DeviceSetting.height = height;

		g_RDX11Device.SetViewport( (float)width, (float)height);

		return g_RenderTargetMgr.Resize(width, height, false);
	}

	bool StartUp(const CENGINE_INIT_PARAM* pParam, IEngine* pEngine)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = pParam->width;
		swapChainDesc.BufferDesc.Height = pParam->height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = (HWND)pParam->hWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = true;

		HRESULT hr = S_OK;
		ID3D11DeviceContext* pImmediateContext = NULL;

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE( driverTypes );

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE( featureLevels );
		D3D_FEATURE_LEVEL	featureLevel;
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			hr = D3D11CreateDeviceAndSwapChain( NULL, 
				driverTypes[driverTypeIndex], 
				NULL, 
				createDeviceFlags, 
				featureLevels, 
				numFeatureLevels,
				D3D11_SDK_VERSION, 
				&swapChainDesc, 
				&g_RenderTargetMgr.pSwapChain, 
				&g_D3Device, 
				&featureLevel, 
				&g_D3DeviceContext );
			if( SUCCEEDED( hr ) )
				break;
		}
		TDXERROR(hr);

		// set global variables
		g_pEngine = pEngine;

		g_DeviceSetting.width = pParam->width;
		g_DeviceSetting.height = pParam->height;

		g_StateRepository.Init();

		g_RenderTargetMgr.CreateMainFrameTarget();
		g_RenderTargetMgr.CreateDefferedTarget(g_DeviceSetting.width, g_DeviceSetting.height);

		g_RDX11Device.SetViewport( (float)g_DeviceSetting.width, (float)g_DeviceSetting.height);

		// initialize subsystem
		g_ShaderMgr.init();
		g_FontRenderer.SetFontFile( "Data\\font\\Font.dds");

		g_RDX11Device.SetRenderStrategy(RS_DEFFERED);

		return true;
	}

	void ShutDown()
	{
		g_FontRenderer.Destroy();
		g_RenderHelper.Destroy();
		g_RenderTargetMgr.Destroy();
		g_StateRepository.Destroy();
		g_ShaderMgr.Destroy();
		SAFE_RELEASE( g_D3DeviceContext );
		SAFE_RELEASE( g_D3Device );
	}
}