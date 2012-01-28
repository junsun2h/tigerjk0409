#pragma once

#include "RDX11Shader.h"

class CResourceMtrl;

class RDX11ShaderGPassLambert : public RDX11Shader
{
public:
	RDX11ShaderGPassLambert();


	void	SetShaderContants(CResourceMtrl* pMaterial, IEntityProxyRender* pRenderProxy) override;
};