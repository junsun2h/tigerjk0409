#pragma once


//------------------------------------------------------------------------------------------------------------
// Vertex Shader
//------------------------------------------------------------------------------------------------------------
struct RDX11VSMPassBase : public RDX11VertexShaderBase
{
	void	SetShaderContants(const XMMATRIX& tm) override;
};


struct RDX11VSMPassColor : public RDX11VSMPassBase
{
	RDX11VSMPassColor();
};


struct RDX11VSMPassFont : public RDX11VSMPassBase
{
	RDX11VSMPassFont();
};



//------------------------------------------------------------------------------------------------------------
// Pixel Shader
//------------------------------------------------------------------------------------------------------------
struct RDX11PSMPassBase : public RDX11PixelShaderBase
{
	RDX11PSMPassBase();
};

struct RDX11PSMPassColor : public RDX11PixelShaderBase
{
	RDX11PSMPassColor();
};

struct RDX11PSMPassFont : public RDX11PixelShaderBase
{
	RDX11PSMPassFont();
};