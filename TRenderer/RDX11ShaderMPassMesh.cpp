#include "CMathType.h"
#include "CCamera.h"
#include "IRDevice.h"

#include "RDX11Global.h"
#include "RDX11Shader.h"
#include "RDX11ShaderMPassMesh.h"


RDX11ShaderMPassMesh::RDX11ShaderMPassMesh()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\MPassMesh.vs";
	desc.szEntrypoint = "VS";
	desc.szShaderModel = "vs_4_0";
	desc.pLayout = layout;
	desc.layoutSize = ARRAYSIZE( layout );
	desc.debugName = "ShaderGPassLambert_VS";

	CreateVS(desc);

	desc.szFileName = "Shader\\MPassMesh.ps";
	desc.szEntrypoint = "PS";
	desc.szShaderModel = "ps_4_0";

	CreatePS(desc);

	GRAPHIC_DEVICE_DESC renderDesc;
	renderDesc.DepthStencil = DEPTH_STENCIL_OFF;
	renderDesc.RasterizerState = RASTERIZER_CULL_BACK;
	renderDesc.BlendState = BLEND_NONE;

	SetRenderState(renderDesc);
	SetTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void RDX11ShaderMPassMesh::SetShaderContants(XMMATRIX& tm)
{
	struct TModelVS
	{
		XMMATRIX wvp;
	}modelVS;
	
	const CCAMERA_DESC* pCamera = GLOBAL::CameraDesc();

	modelVS.wvp = XMMatrixMultiply( tm, pCamera->ViewTM ); 
	modelVS.wvp = XMMatrixMultiply( modelVS.wvp, pCamera->ProjTM ); 
	modelVS.wvp = XMMatrixTranspose( modelVS.wvp );

	GLOBAL::ShaderMgr()->UpdateShaderConstant( &modelVS, sizeof( TModelVS), SM_BUF11_192BYTE_SLOT0, VS_SHADER );
}