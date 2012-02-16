#include "RDefine.h"

#include "CMathType.h"
#include "CCamera.h"

#include "IRDX11Device.h"
#include "IRenderStateMgr.h"
#include "IShader.h"

#include "RDX11Global.h"
#include "RDX11Shader.h"
#include "RDX11ShaderMPassMesh.h"


RDX11ShaderMPassMesh::RDX11ShaderMPassMesh()
{
	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\MPassMesh.vs";
	desc.szEntrypoint = "VS";
	desc.szShaderModel = "vs_4_0";
	desc.eVertexyType = FVF_3FP_1DC;

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