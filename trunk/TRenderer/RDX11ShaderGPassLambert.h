#pragma once


class RDX11ShaderGPassLambert : public RDX11ShaderBase
{
public:
	RDX11ShaderGPassLambert();


	void	SetShaderContants(XMMATRIX& tm) override;
};