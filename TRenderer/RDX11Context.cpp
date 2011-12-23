#include "RDX11Context.h"
#include "RDX11Window.h"





RDX11Context::RDX11Context()
{
}

RDX11Context::~RDX11Context()
{

}


void RDX11Context::SetViewport(float width, float height, float MinDepth, float MaxDepth, float TopLeftX, float TopLeftY)
{
	D3D11_VIEWPORT vp;

	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = MinDepth;
	vp.MaxDepth = MaxDepth;
	vp.TopLeftX = TopLeftX;
	vp.TopLeftY = TopLeftY;

	m_pContext->RSSetViewports( 1, &vp );
}

void RDX11Context::SetRasterizerState(ID3D11RasterizerState* pState)
{
	m_pContext->RSSetState(pState); 
}

void RDX11Context::SetDepthStencilState(ID3D11DepthStencilState* pState)
{
	m_pContext->OMSetDepthStencilState(pState, NULL); 
}

void RDX11Context::SetBlendState(ID3D11BlendState* pState)
{
	m_pContext->OMSetBlendState(pState, NULL, 0xffffffff);
}

void RDX11Context::SetSampler(ID3D11SamplerState* pState, int slot)
{
	m_pContext->PSSetSamplers( slot, 1, &pState );
}

void RDX11Context::SetDefaultState(RDX11StateRepository* pStateRepository)
{
	SetRasterizerState( pStateRepository->GetRasterizerState( RASTERIZER_CULL_BACK ) );
	SetSampler( pStateRepository->GetSamplerState(ANSIO16_SAMPLER), 0);
	SetSampler( pStateRepository->GetSamplerState(LNEAR_SAMPLER), 1);
	SetSampler( pStateRepository->GetSamplerState(POINT_SAMPLER), 2);
}

void RDX11Context::SetTarget(RDX11Window* pWindow)
{
	m_pContext->OMSetRenderTargets( 1, &pWindow->pRTV, pWindow->pDSV );
	m_pContext->ClearRenderTargetView( pWindow->pRTV, (float*)CVector4(0.25f, 0.25f, 0.55f, 1.0f) );
}

void RDX11Context::Destroy()
{
	m_pContext->Release();
}