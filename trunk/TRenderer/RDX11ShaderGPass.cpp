#include "RDefine.h"

#include "CResource.h"
#include "CCamera.h"

#include "IRDX11Device.h"
#include "IEntityProxy.h"
#include "IRenderStateMgr.h"
#include "IShader.h"


#include "RDX11Global.h"
#include "RDX11Shader.h"
#include "RDX11ShaderGPass.h"


void RDX11VSGPassBase::SetShaderContants(const XMMATRIX& tm)
{
	struct TModelVS
	{
		XMMATRIX wvp;
		XMMATRIX wv;
	}modelVS;

	const CCAMERA_DESC* pCamera = GLOBAL::CameraDesc();

	modelVS.wv = XMMatrixMultiply( tm, pCamera->ViewTM ); 
	modelVS.wvp = XMMatrixMultiply( modelVS.wv, pCamera->ProjTM ); 

	modelVS.wv = XMMatrixTranspose( modelVS.wv );
	modelVS.wvp = XMMatrixTranspose( modelVS.wvp );

	GLOBAL::ShaderMgr()->UpdateShaderConstant( &modelVS, sizeof( TModelVS), SM_BUF11_192BYTE_SLOT0, VERTEX_SHADER );
}


//------------------------------------------------------------------------------------------------------------
// Vertex Shader
//------------------------------------------------------------------------------------------------------------
RDX11VSGPassLambert::RDX11VSGPassLambert()
{
	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\GPass.vs";
	desc.szEntrypoint = "VS";
	desc.szShaderModel = "vs_4_0";
	desc.eVertexyType = FVF_4HP_4BN_2HT;

	CreateVS(desc);
}


RDX11VSGPassLambertWeight::RDX11VSGPassLambertWeight()
{
	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\GPass.vs";
	desc.szEntrypoint = "VS_SKIN";
	desc.szShaderModel = "vs_4_0";
	desc.eVertexyType = FVF_4HP_4BN_2HT_4BW;

	CreateVS(desc);
}


RDX11VSGPassNormalMap::RDX11VSGPassNormalMap()
{
	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\GPass.vs";
	desc.szEntrypoint = "VS";
	desc.szShaderModel = "vs_4_0";
	desc.eVertexyType = FVF_4HP_4BN_2HT;

	CreateVS(desc);
}

RDX11VSGPassNormalMapWeight::RDX11VSGPassNormalMapWeight()
{
	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\GPass.vs";
	desc.szEntrypoint = "VS";
	desc.szShaderModel = "vs_4_0";
	desc.eVertexyType = FVF_4HP_4BN_2HT;

	CreateVS(desc);
}



//------------------------------------------------------------------------------------------------------------
// Pixel Shader
//------------------------------------------------------------------------------------------------------------
RDX11PSGPassBase::RDX11PSGPassBase()
{
	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\GPass.ps";
	desc.szEntrypoint = "PS";
	desc.szShaderModel = "ps_4_0";

	CreatePS(desc);

	GRAPHIC_DEVICE_DESC renderDesc;
	renderDesc.DepthStencil = DEPTH_STENCIL_WRITE;
	renderDesc.RasterizerState = RASTERIZER_CULL_BACK;
	renderDesc.BlendState = BLEND_NONE;

	SetRenderState( renderDesc );
}