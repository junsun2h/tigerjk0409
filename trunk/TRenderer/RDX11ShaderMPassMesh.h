#pragma once

class RDX11ShaderMPassMesh : public RDX11ShaderBase
{
public:
	RDX11ShaderMPassMesh();

	void		SetShaderContants(XMMATRIX& tm) override;
};