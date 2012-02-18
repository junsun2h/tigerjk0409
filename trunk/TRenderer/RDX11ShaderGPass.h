#pragma once


//------------------------------------------------------------------------------------------------------------
// Vertex Shader
//------------------------------------------------------------------------------------------------------------
struct RDX11VSGPassBase : public RDX11VertexShaderBase
{
	void	SetShaderContants(const XMMATRIX& tm) override;
};

struct RDX11VSGPassLambert : public RDX11VSGPassBase
{
	RDX11VSGPassLambert();
};


struct RDX11VSGPassLambertWeight : public RDX11VSGPassBase
{
	RDX11VSGPassLambertWeight();

	void	SetShaderContants(XMMATRIX* tm, UINT size) override;
};


struct RDX11VSGPassNormalMap : public RDX11VSGPassBase
{
	RDX11VSGPassNormalMap();
};


struct RDX11VSGPassNormalMapWeight : public RDX11VSGPassBase
{
	RDX11VSGPassNormalMapWeight();

	void	SetShaderContants(XMMATRIX* tm, UINT size) override;
};





//------------------------------------------------------------------------------------------------------------
// Pixel Shader
//------------------------------------------------------------------------------------------------------------
struct RDX11PSGPassBase : public RDX11PixelShaderBase
{
	RDX11PSGPassBase();
};