#pragma once

#include "RDefine.h"
#include "RDX11RenderStateMgr.h"

class CResourceMtrl;
struct IEntityProxyRender;



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

class RDX11Shader
{
public:
	RDX11Shader();
	virtual ~RDX11Shader();

	virtual void		Begin();
	virtual void		End(){}

	virtual void		SetShaderContants(IEntityProxyRender* pRenderProxy){}
	virtual void		SetShaderContants(CResourceMtrl* pMaterial){}
	virtual void		SetShaderContants(XMMATRIX& tm){}

	void				CreateVS( SHADER_COMPILE_DESC& desc);
	void				CreatePS( SHADER_COMPILE_DESC& desc);

	void				SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)		{ m_Topology = topology; }
	void				SetRenderState(	DEPTH_STENCIL_TYPE DepthStencilState,
										RASTERIZER_TYPE RasterizerState,
										ALPHA_BLEND_TYPE BlendState,
										UINT StencilRef = NULL,
										float* blendFactor = NULL,
										UINT sampleMask = 0xffffffff);

	void				Destroy();


private:
	
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;	
	ID3D11GeometryShader*		m_pGeometryShader;
	
	ID3D11InputLayout*			m_pVertexLayout;

	D3D_PRIMITIVE_TOPOLOGY		m_Topology;

	DEPTH_STENCIL_TYPE			m_DepthStencilState;
	RASTERIZER_TYPE				m_RasterizerState;
	ALPHA_BLEND_TYPE			m_BlendState;
	UINT						m_StencilRef;
	float						m_BlendFactor[4];
	UINT						m_SampleMask;
};