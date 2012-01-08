#pragma once

#include "IRDevice.h"
#include "RDefine.h"
#include "RDX11RenderStateMgr.h"
#include "RDX11Window.h"
#include "RDX11Font.h"
#include "RDX11RenderHelper.h"
#include "RDX11ShaderMgr.h"
#include "CThread.h"
#include "IEngine.h"
#include <CGuid.h>
#include <atlcoll.h>



struct RDX11Setting
{
	D3D_FEATURE_LEVEL	featureLevel;
	D3D_DRIVER_TYPE		driverType;
	int					width;
	int					height;
};




class RDX11Device : public IRDevice
{
	CCriticalSectionSpinLock m_CS;

public:
	RDX11Device();
	~RDX11Device();

	/////////////////////////////////////////////////////////////////////////
	// IRenderer
	//////////////////////////////////////////////////////////////////////////
	bool		StartUp(const CENGINE_INIT_PARAM &param) override;
	void		ShutDown() override;
	void		Render(const CCAMERA_DESC& cameraDesc) override;
	void		Present() override;

	bool		Resize(int width, int height) override;
	HWND		GetHWND() override	{ return m_HWND; }
	void		TS_CreateDPResource(DEVICE_DEPENDENT_RESOURCE type, void* pBuf ,int size, IResource* pResource) override;
	
	IRenderHelper*	GetRenderHelper() override;

public:
	void	SetViewport(float width, float height, float MinDepth = 0.0f, float MaxDepth = 1.0f, float TopLeftX = 0.0f, float TopLeftY= 0.0f);

private:
	void		TS_CreateVB(void* pBuf ,int size, IResource* pResource);
	void		TS_CreateVBOut(void* pBuf ,int size, IResource* pResource);
	void		TS_CreateIB(void* pBuf ,int size, IResource* pResource);
	void		TS_CreateTexture(void* pBuf ,int size, IResource* pResource);
	void		TS_CreateShader(void* pBuf ,int size, IResource* pResource);

	HWND						m_HWND;
	
	ID3D11Device*				m_pD3Device;
	ID3D11DeviceContext*		m_pContext;

	typedef ATL::CAtlMap<long, ID3D11Buffer*>				RDX11BUFFER_RESOURCE_MAP;
	typedef ATL::CAtlMap<long, ID3D11ShaderResourceView*>	RDX11TEXTURE_RESOURCE_MAP;
	typedef ATL::CAtlMap<long, ID3D11DeviceChild*>			RDX11SHADER_RESOURCE_MAP;

	RDX11BUFFER_RESOURCE_MAP	m_RscVBMap;
	RDX11BUFFER_RESOURCE_MAP	m_RscIBMap;
	RDX11TEXTURE_RESOURCE_MAP	m_RscTexMap;
	RDX11SHADER_RESOURCE_MAP	m_RscShaderMap;
};


namespace GLOBAL
{
	ID3D11Device*			GetD3DDevice();
	ID3D11DeviceContext*	GetD3DContext();
	RDX11Device*			GetRDX11Device();
	RDX11RenderStateMgr*	GetD3DStateMgr();
	const RDX11Setting&		GetDeviceInfo();
	RDX11ShaderMgr*			GetShaderMgr();
	const CCAMERA_DESC&		GetCameraDesc();
};
