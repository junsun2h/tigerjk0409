#include "CResource.h"
#include "CCamera.h"

#include "IRDevice.h"
#include "IEntityProxy.h"

#include "RDX11Global.h"
#include "RDX11Shader.h"
#include "RDX11ShaderGPassLambert.h"


RDX11ShaderGPassLambert::RDX11ShaderGPassLambert()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R8G8B8A8_SINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\GPassLambert.vs";
	desc.szEntrypoint = "VS";
	desc.szShaderModel = "vs_4_0";
	desc.pLayout = layout;
	desc.layoutSize = ARRAYSIZE( layout );
	desc.debugName = "ShaderGPassLambert_VS";

	CreateVS(desc);

	desc.szFileName = "Shader\\GPassLambert.ps";
	desc.szEntrypoint = "PS";
	desc.szShaderModel = "ps_4_0";

	CreatePS(desc);

	GRAPHIC_DEVICE_DESC renderDesc;
	renderDesc.DepthStencil = DEPTH_STENCIL_WRITE;
	renderDesc.RasterizerState = RASTERIZER_CULL_BACK;
	renderDesc.BlendState = BLEND_NONE;

	SetRenderState( renderDesc );
	SetTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void RDX11ShaderGPassLambert::SetShaderContants(XMMATRIX& tm)
{
	struct TModelVS
	{
		XMMATRIX wvp;
		XMMATRIX wv;
		XMMATRIX world;
	}modelVS;
	
	const CCAMERA_DESC* pCamera = GLOBAL::CameraDesc();

	modelVS.world = tm;
	modelVS.wv = XMMatrixMultiply( modelVS.world, pCamera->ViewTM ); 
	modelVS.wvp = XMMatrixMultiply( modelVS.wv, pCamera->ProjTM ); 

	modelVS.world = XMMatrixTranspose( modelVS.world );
	modelVS.wv = XMMatrixTranspose( modelVS.wv );
	modelVS.wvp = XMMatrixTranspose( modelVS.wvp );

	GLOBAL::ShaderMgr()->UpdateShaderConstant( &modelVS, sizeof( TModelVS), SM_BUF11_192BYTE_SLOT0, VS_SHADER );
}