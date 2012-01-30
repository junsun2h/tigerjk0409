#pragma once

#include "RDX11Shader.h"

class CResourceMtrl;

class RDX11ShaderMPassMesh : public RDX11Shader
{
public:
	RDX11ShaderMPassMesh();

	void		SetShaderContants(CResourceMtrl* pMaterial, XMMATRIX& tm) override;
};