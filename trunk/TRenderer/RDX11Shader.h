#pragma once

#include "RDefine.h"
#include "RDX11RenderStateMgr.h"


class RDX11Shader
{
public:
	RDX11Shader();
	virtual ~RDX11Shader();

	void		SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)		{ m_Topology = topology; }
	
	void		Begin();
	void		End();

	void		CreateVS( void* pSrc, 
							SIZE_T SrcDataSize,
							LPCSTR pEntrypoint, 
							LPCSTR pTarget, 
							UINT flag,
							D3D11_INPUT_ELEMENT_DESC layout[], 
							UINT layoutSize,
							LPCSTR debugName = "");


	void		CreatePS( void* pSrc, 
							SIZE_T SrcDataSize,
							LPCSTR pEntrypoint, 
							LPCSTR pTarget, 
							UINT flag,
							LPCSTR debugName = "");


	void		SetRenderState(	DEPTH_STENCIL_TYPE DepthStencilState,
								RASTERIZER_TYPE RasterizerState,
								ALPHA_BLEND_TYPE BlendState,
								UINT StencilRef = NULL,
								float* blendFactor = NULL,
								UINT sampleMask = 0xffffffff);
	
	void		ApplyRenderState();

	void		Destroy();


private:
	
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;	
	ID3D11GeometryShader*		m_pGeometryShader;
	
	ID3D11InputLayout*			m_pVertexLayout;
	ID3D11InputLayout*			m_pVertexLayoutPos4;

	D3D_PRIMITIVE_TOPOLOGY		m_Topology;

	DEPTH_STENCIL_TYPE			m_DepthStencilState;
	RASTERIZER_TYPE				m_RasterizerState;
	ALPHA_BLEND_TYPE			m_BlendState;
	UINT						m_StencilRef;
	float						m_BlendFactor[4];
	UINT						m_SampleMask;
};