#pragma once

#include "IRDevice.h"
#include "RDefine.h"
#include "RDX11RenderStateMgr.h"
#include "RDX11RenderTargetMgr.h"
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
	bool		StartUp(const CENGINE_INIT_PARAM &param, IEngine* pEngine) override;
	void		ShutDown() override;
	void		Render(const CCAMERA_DESC& cameraDesc) override;
	void		Present() override;

	bool		Resize(int width, int height) override;
	HWND		GetHWND() override	{ return m_HWND; }
	void		PT_CreateGraphicBuffer(CResourceBase* pResource) override;
	void		CreateGraphicBuffer(CResourceBase* pResource) override;
	void		RemoveGraphicBuffer(CResourceBase* pResource) override;

	IRenderHelper*	GetRenderHelper() override;

public:
	void			SetViewport(float width, float height, float MinDepth = 0.0f, float MaxDepth = 1.0f, float TopLeftX = 0.0f, float TopLeftY= 0.0f);
	ID3D11Buffer*	CreateBuffer(void* pData ,int size, UINT bindFlag, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
	void			RecreateBuffer(ID3D11Buffer** ppBuffer, void* pData ,int size, UINT bindFlag, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

private:


	HWND					m_HWND;
	
	ID3D11Device*			m_pD3Device;
	ID3D11DeviceContext*	m_pContext;
};


namespace GLOBAL
{
	IAssetMgr*				GetAssetMgr();
	ID3D11Device*			GetD3DDevice();
	ID3D11DeviceContext*	GetD3DContext();
	RDX11Device*			GetRDX11Device();
	RDX11RenderStateMgr*	GetD3DStateMgr();
	const RDX11Setting&		GetDeviceInfo();
	RDX11ShaderMgr*			GetShaderMgr();
	const CCAMERA_DESC&		GetCameraDesc();
};
