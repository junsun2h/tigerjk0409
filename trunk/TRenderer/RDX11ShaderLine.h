#pragma once


class RDX11ShaderLine : public RDX11ShaderBase
{
public:
	RDX11ShaderLine();

	void		SetShaderContants(XMMATRIX& tm) override;
};