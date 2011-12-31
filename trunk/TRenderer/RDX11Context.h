#pragma once

#include "RDefine.h"
#include "RDX11StateRepository.h"

struct RDX11Window;


class RDX11Context
{
	ID3D11DeviceContext*		m_pContext;
	
	// stored states
	ID3D11DepthStencilState*	m_pDepthStencilStateStored11;
	UINT						m_StencilRefStored11;
	ID3D11RasterizerState*		m_pRasterizerStateStored11;
	ID3D11BlendState*			m_pBlendStateStored11;
	float						m_BlendFactorStored11[4];
	UINT						m_SampleMaskStored11;
	ID3D11SamplerState*			m_pSamplerStateStored11;

public:
	RDX11Context();
	~RDX11Context();

	ID3D11DeviceContext**	GetContextP()		{ return &m_pContext; }	
	ID3D11DeviceContext*	GetContext()		{ return m_pContext; }	


	void	SetViewport(float width, float height, float MinDepth = 1.0f, float MaxDepth = 1.0f, float TopLeftX = 0.0f, float TopLeftY= 0.0f);
	void	SetRasterizerState(ID3D11RasterizerState* pState);
	void	SetDepthStencilState(ID3D11DepthStencilState* pState);
	void	SetBlendState(ID3D11BlendState* pState);
	void	SetSampler(ID3D11SamplerState* pState, int slot);
	void	SetDefaultState(RDX11StateRepository* pStateRepository);
	void	SetTarget(RDX11Window* pWindow);

	void	Destroy();

	void	StoreCurrentState();
	void	RestoreSavedState();
};
