#pragma once



class RDX11Device : public IRDevice
{
public:
	RDX11Device();
	~RDX11Device();

	/////////////////////////////////////////////////////////////////////////
	// IRDevice
	//////////////////////////////////////////////////////////////////////////
	bool				StartUp(const CENGINE_INIT_PARAM* pParam, IEngine* pEngine) override;
	void				ShutDown() override;

	void				Present() override;

	bool				Resize(int width, int height) override;
	HWND				GetHWND() override	{ return m_HWND; }
	void				CreateGraphicBuffer(CResourceBase* pResource) override;
	void				RemoveGraphicBuffer(CResourceBase* pResource) override;
	
	CResourceTexture*	CreateTextureFromFile(const char* fileName, CResourceTexture* pTexture) override;
	bool				SaveTextureToFile(const CResourceTexture* pTexture, eIMAGE_FILE_FORMAT format, const char* fileName) override;

	IRenderHelper*		GetRenderHelper() override;
	IRenderStrategy*	GetRenderer() override				{ return m_pCurrentRenderStrategy; }

	RDeviceDesc			GetDeviceSetting() override;

public:
	void				SetRenderStrategy(eRENDER_STRATEGY strategy);
	void				SetViewport(float width, float height, float MinDepth = 0.0f, float MaxDepth = 1.0f, float TopLeftX = 0.0f, float TopLeftY= 0.0f);
	ID3D11Buffer*		CreateBuffer(void* pData ,int size, UINT bindFlag, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

	void				CreateDXGeometryBuffer(CResourceGeometry* pGeometry);
	void				CreateDXTextureBuffer(CResourceTexture* pTexture);

private:
	HWND				m_HWND;

	IRenderStrategy*	m_pCurrentRenderStrategy;
};