#include "RDX11ShaderGPassLambert.h"
#include "CResource.h"
#include "IEntityProxy.h"
#include "IEntity.h"
#include "RDX11Device.h"


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

	SetRenderState(DEPTH_STENCIL_WRITE, RASTERIZER_CULL_BACK, BLEND_NONE);
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
	
	const CCAMERA_DESC& camera = GLOBAL::GetCameraDesc();

	modelVS.world = tm;
	modelVS.wv = XMMatrixMultiply( modelVS.world, camera.ViewTM ); 
	modelVS.wvp = XMMatrixMultiply( modelVS.wv, camera.ProjTM ); 

	modelVS.world = XMMatrixTranspose( modelVS.world );
	modelVS.wv = XMMatrixTranspose( modelVS.wv );
	modelVS.wvp = XMMatrixTranspose( modelVS.wvp );

	GLOBAL::GetShaderMgr()->UpdateShaderConstant( &modelVS, sizeof( TModelVS), SM_BUF11_192BYTE_SLOT0, VS_SHADER );
}