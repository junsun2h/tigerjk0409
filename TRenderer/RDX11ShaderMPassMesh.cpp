#include "RDX11ShaderMPassMesh.h"
#include "CResource.h"
#include "IEntityProxy.h"
#include "IEntity.h"
#include "RDX11Device.h"


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

	SetRenderState(DEPTH_STENCIL_OFF, RASTERIZER_CULL_BACK, BLEND_NONE);
	SetTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void RDX11ShaderMPassMesh::SetShaderContants(XMMATRIX& tm)
{
	struct TModelVS
	{
		XMMATRIX wvp;
	}modelVS;
	
	const CCAMERA_DESC& camera = GLOBAL::GetCameraDesc();

	modelVS.wvp = XMMatrixMultiply( tm, camera.ViewTM ); 
	modelVS.wvp = XMMatrixMultiply( modelVS.wvp, camera.ProjTM ); 
	modelVS.wvp = XMMatrixTranspose( modelVS.wvp );

	GLOBAL::GetShaderMgr()->UpdateShaderConstant( &modelVS, sizeof( TModelVS), SM_BUF11_192BYTE_SLOT0, VS_SHADER );
}