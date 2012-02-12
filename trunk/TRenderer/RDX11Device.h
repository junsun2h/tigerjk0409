#pragma once



class RDX11Device : public IRDX11Device
{
public:
	RDX11Device();
	~RDX11Device();

	/////////////////////////////////////////////////////////////////////////
	// IRDevice
	//////////////////////////////////////////////////////////////////////////
	bool				StartUp(const CENGINE_INIT_PARAM* pParam, IEngine* pEngine) override;
	void				ShutDown() override;

	void				RenderFrame(CCAMERA_DESC* pCameraDesc) override;
	void				RenderElement(	CResourceGeometry*	pGeometry, CResourceMtrl* pMtrl, IEntityProxyRender* pRenderProxy) override;
	void				Present() override;

	bool				Resize(int width, int height) override;
	HWND				GetHWND() override	{ return m_HWND; }
	void				CreateGraphicBuffer(CResourceBase* pResource) override;
	void				RemoveGraphicBuffer(CResourceBase* pResource) override;
	
	CResourceTexture*	CreateTextureFromFile(const char* fileName, CResourceTexture* pTexture) override;
	bool				SaveTextureToFile(const CResourceTexture* pTexture, eIMAGE_FILE_FORMAT format, const char* fileName) override;

	IRenderHelper*		GetRenderHelper() override;
	RDeviceDesc			GetDeviceSetting() override;
	
	eRENDER_PASS		GetCurrentPass() override			{ return m_currentRenderPass; }
	eRENDER_PASS		SetCurrentPass(eRENDER_PASS pass)	{ m_currentRenderPass = pass; }

public:
	/////////////////////////////////////////////////////////////////////////
	// IRDX11Device
	//////////////////////////////////////////////////////////////////////////
	void				RecreateBuffer(ID3D11Buffer** ppBuffer, void* pData ,int size, UINT bindFlag, D3D11_USAGE usage = D3D11_USAGE_DEFAULT) override;
	void				RenderGeometry(CResourceGeometry* pGeometry) override;

public:
	void				SetRenderStrategy(eRENDER_STRATEGY strategy);
	void				SetViewport(float width, float height, float MinDepth = 0.0f, float MaxDepth = 1.0f, float TopLeftX = 0.0f, float TopLeftY= 0.0f);
	ID3D11Buffer*		CreateBuffer(void* pData ,int size, UINT bindFlag, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

	void				CreateDXGeometryBuffer(CResourceGeometry* pGeometry);
	void				CreateDXTextureBuffer(CResourceTexture* pTexture);

private:
	HWND				m_HWND;

	eRENDER_PASS		m_currentRenderPass;
	IRenderStrategy*	m_pCurrentRenderStrategy;
};