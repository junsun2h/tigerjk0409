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

	void		RenderFrame(const CCAMERA_DESC& cameraDesc) override;
	void		RenderElement(	CResourceGeometry*	pGeometry, CResourceMtrl* pMtrl, IEntityProxyRender* pRenderProxy) override;
	void		RenderGeometry(CResourceGeometry*	pGeometry);
	void		Present() override;

	bool		Resize(int width, int height) override;
	HWND		GetHWND() override	{ return m_HWND; }
	void		PT_CreateGraphicBuffer(CResourceBase* pResource) override;
	void		CreateGraphicBuffer(CResourceBase* pResource) override;
	void		RemoveGraphicBuffer(CResourceBase* pResource) override;
	
	CResourceTexture*	CreateTextureFromFile(const char* fileName) override;
	bool				SaveTextureToFile(const CResourceTexture* pTexture, eIMAGE_FILE_FORMAT format, const char* fileName) override;

	IRenderHelper*	GetRenderHelper() override;
	RDeviceDesc		GetDeviceSetting() override;
	
	eRENDER_PASS	GetCurrentPass() override			{ return m_currentRenderPass; }
	eRENDER_PASS	SetCurrentPass(eRENDER_PASS pass)	{ m_currentRenderPass = pass; }


	void			SetRenderStrategy(eRENDER_STRATEGY strategy);

public:
	void			SetViewport(float width, float height, float MinDepth = 0.0f, float MaxDepth = 1.0f, float TopLeftX = 0.0f, float TopLeftY= 0.0f);
	ID3D11Buffer*	CreateBuffer(void* pData ,int size, UINT bindFlag, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
	void			RecreateBuffer(ID3D11Buffer** ppBuffer, void* pData ,int size, UINT bindFlag, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

private:
	HWND					m_HWND;
	
	ID3D11Device*			m_pD3Device;
	ID3D11DeviceContext*	m_pContext;
	D3D_FEATURE_LEVEL		m_FeatureLevel;
	eRENDER_PASS			m_currentRenderPass;
	IRenderStrategy*		m_pCurrentRenderStrategy;
};

	
class RDX11MultyThreadRenderer;

namespace GLOBAL
{
	IEngine*					Engine();
	ID3D11Device*				GetD3DDevice();
	ID3D11DeviceContext*		GetD3DContext();
	RDX11Device*				GetRDX11Device();
	RDX11RenderStateMgr*		GetD3DStateMgr();
	const RDeviceDesc&			GetDeviceInfo();
	RDX11ShaderMgr*				GetShaderMgr();
	const CCAMERA_DESC&			GetCameraDesc();
	RDX11RenderTargetMgr*		GetMainFrameRenderTarget();
	RDX11MultyThreadRenderer*	GetMultyThreadRenderer();
};
