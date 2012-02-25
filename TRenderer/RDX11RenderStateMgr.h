#pragma once



class RDX11RenderStateMgr : public IRenderStateMgr
{
public:
	RDX11RenderStateMgr();
	~RDX11RenderStateMgr();

	virtual void	Init() override;
	virtual void	Destroy() override;

	virtual void	StoreCurrentState() override 	{	m_TemporaySavedDesc = m_CurrentDesc;	}
	virtual void	RestoreSavedState() override	{	ApplyRenderState(m_TemporaySavedDesc);	}

	virtual void	ApplyRenderState(const GRAPHIC_DEVICE_DESC& desc) override;
	virtual void	SetRasterizer(RASTERIZER_TYPE RasterizerState) override;
	virtual void	SetBlend(ALPHA_BLEND_TYPE BlendState, float* pblendFactor = NULL, UINT sampleMask = 0xffffffff) override;
	virtual void	SetDepthStancil(DEPTH_STENCIL_TYPE DepthStencilState, UINT StencilRef = NULL) override;

	virtual void	SetVertexInput(eCVERTEX_TYPE type) override;
	virtual void	CreateInputLayout(eCVERTEX_TYPE	eVertexyType, ID3DBlob* pBlob) override;

	virtual void	SetTopology(D3D_PRIMITIVE_TOPOLOGY topology) override;

private:
	void						CreateSampler(ID3D11Device* pD3Device);
	void						CreateBlenderStates(ID3D11Device* pD3Device);
	void						CreateRasterizerStates(ID3D11Device* pD3Device);
	void						CreateDepthStencilStates(ID3D11Device* pD3Device);

	ID3D11BlendState*			m_pBlendType[NUM_ALPHA_BLEND_TYPE];
	ID3D11RasterizerState*		m_pRasterizerType[NUM_RASTERIZER_TYPE];
	ID3D11DepthStencilState*	m_pDepthStencilType[NUM_DEPTH_STENCIL];
	ID3D11SamplerState*			m_SamplerStates[NUM_SAMPLER_TYPE];

	bool						m_bCreated;
	GRAPHIC_DEVICE_DESC			m_CurrentDesc;
	GRAPHIC_DEVICE_DESC			m_TemporaySavedDesc;

	typedef ATL::CAtlMap<eCVERTEX_TYPE, ID3D11InputLayout*>	INPUT_LAYOUT_MAP;
	INPUT_LAYOUT_MAP			m_InputLayoutMap;

	eCVERTEX_TYPE				m_currentVertexType;
	D3D_PRIMITIVE_TOPOLOGY		m_currentTopology;
};