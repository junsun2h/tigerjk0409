#pragma once


struct SHADER_COMPILE_DESC
{
	LPCSTR		szFileName; 
	void*		pSrc; 
	SIZE_T		SrcDataSize;
	LPCSTR		szEntrypoint; 
	LPCSTR		szShaderModel; 
	UINT		flag;

	D3D11_INPUT_ELEMENT_DESC*	pLayout;
	UINT						layoutSize;

	LPCSTR		debugName;

	SHADER_COMPILE_DESC()
	{
		memset( this, 0, sizeof(SHADER_COMPILE_DESC));
	}
};


class RDX11ShaderBase : public IShader
{
public:
	RDX11ShaderBase();
	virtual ~RDX11ShaderBase();

	virtual void		Begin() override;
	virtual void		End() override {}

	virtual void		SetShaderContants(IEntityProxyRender* pRenderProxy) override {}
	virtual void		SetShaderContants(CResourceMtrl* pMaterial) override {}
	virtual void		SetShaderContants(XMMATRIX& tm) override {}

public:
	void				CreateVS( SHADER_COMPILE_DESC& desc);
	void				CreatePS( SHADER_COMPILE_DESC& desc);

	void				SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)		{ m_Topology = topology; }
	void				SetRenderState(	const GRAPHIC_DEVICE_DESC desc);

	void				ApplyRenderState();
	void				Destroy();


private:
	
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;	
	ID3D11GeometryShader*		m_pGeometryShader;
	
	ID3D11InputLayout*			m_pVertexLayout;

	D3D_PRIMITIVE_TOPOLOGY		m_Topology;

	GRAPHIC_DEVICE_DESC			m_RenderState;
};