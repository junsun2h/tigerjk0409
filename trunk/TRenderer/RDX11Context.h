#pragma once

#include "RDefine.h"
#include "RDX11StateRepository.h"
#include "CThread.h"


class RDX11Device;
struct RDX11Window;


class RDX11Context
{
	CCriticalSectionSpinLock	m_CS;
	ID3D11DeviceContext*		m_pContext;

public:
	RDX11Context();
	~RDX11Context();

	ID3D11DeviceContext**	GetContextP()		{ return &m_pContext; }	

	void	SetViewport(float width, float height, float MinDepth = 0.0f, float MaxDepth = 0.0f, float TopLeftX = 0.0f, float TopLeftY= 0.0f);
	void	SetRasterizerState(ID3D11RasterizerState* pState);
	void	SetDepthStencilState(ID3D11DepthStencilState* pState);
	void	SetBlendState(ID3D11BlendState* pState);
	void	SetSampler(ID3D11SamplerState* pState, int slot);
	void	SetDefaultState(RDX11StateRepository* pStateRepository);
	void	SetTarget(RDX11Window* pWindow);

	void	Destroy();
};