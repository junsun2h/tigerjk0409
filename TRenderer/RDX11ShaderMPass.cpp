#include "RDefine.h"

#include "CResource.h"
#include "CCamera.h"

#include "IRDevice.h"
#include "IEntityProxy.h"
#include "IRenderStateMgr.h"
#include "IShader.h"


#include "RDX11Global.h"
#include "RDX11Shader.h"
#include "RDX11ShaderMPass.h"


void RDX11VSMPassBase::SetShaderContants(const XMMATRIX& tm)
{
	struct TModelVS
	{
		XMMATRIX wvp;
	}modelVS;

	const CCAMERA_DESC* pCamera = GLOBAL::CameraDesc();

	modelVS.wvp = XMMatrixMultiply( tm, pCamera->ViewTM ); 
	modelVS.wvp = XMMatrixMultiply( modelVS.wvp, pCamera->ProjTM ); 
	modelVS.wvp = XMMatrixTranspose( modelVS.wvp );

	GLOBAL::ShaderMgr()->UpdateShaderConstant( &modelVS, sizeof( TModelVS), SM_BUF11_192BYTE_SLOT0, VERTEX_SHADER );
}

//------------------------------------------------------------------------------------------------------------
// Vertex Shader
//------------------------------------------------------------------------------------------------------------

RDX11VSMPassColor::RDX11VSMPassColor()
{
	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\MPass.vs";
	desc.szEntrypoint = "VS_COLOR";
	desc.szShaderModel = "vs_4_0";
	desc.eVertexyType = FVF_3FP_1DC;

	CreateVS(desc);
}

RDX11VSMPassFont::RDX11VSMPassFont()
{
	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\MPass.vs";
	desc.szEntrypoint = "VS_FONT";
	desc.szShaderModel = "vs_4_0";
	desc.eVertexyType = FVF_3FP_1DC_2HT;

	CreateVS(desc);
}


//------------------------------------------------------------------------------------------------------------
// Pixel Shader
//------------------------------------------------------------------------------------------------------------

RDX11PSMPassColor::RDX11PSMPassColor()
{
	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\MPass.ps";
	desc.szEntrypoint = "PS_COLOR";
	desc.szShaderModel = "ps_4_0";

	CreatePS(desc);

	GRAPHIC_DEVICE_DESC renderDesc;

	renderDesc.DepthStencil = DEPTH_STENCIL_ON;
	renderDesc.RasterizerState = RASTERIZER_CULL_BACK;
	renderDesc.BlendState = BLEND_NONE;

	SetRenderState(renderDesc);
}

RDX11PSMPassFont::RDX11PSMPassFont()
{
	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\MPass.ps";
	desc.szEntrypoint = "PS_FONT";
	desc.szShaderModel = "ps_4_0";

	CreatePS(desc);

	GRAPHIC_DEVICE_DESC renderDesc;

	renderDesc.DepthStencil = DEPTH_STENCIL_OFF;
	renderDesc.RasterizerState = RASTERIZER_CULL_BACK;
	renderDesc.BlendState = BLEND_ADD_BY_ALPHA;

	SetRenderState(renderDesc);
}
