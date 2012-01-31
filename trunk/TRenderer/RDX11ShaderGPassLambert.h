#pragma once

#include "RDX11Shader.h"


class RDX11ShaderGPassLambert : public RDX11Shader
{
public:
	RDX11ShaderGPassLambert();


	void	SetShaderContants(XMMATRIX& tm) override;
};