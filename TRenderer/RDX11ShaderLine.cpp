#include "RDefine.h"

#include "CResource.h"
#include "CCamera.h"

#include "IRDX11Device.h"
#include "IRenderStateMgr.h"
#include "IShader.h"

#include "RDX11Global.h"
#include "RDX11Shader.h"
#include "RDX11ShaderLine.h"


char g_strLineEffectFile[] = \
	"cbuffer CB_Model : register( b11 )"\
	"{"\
	"	matrix WVP;	"\
	"};"\
	""\
	""\
	"struct VS_INPUT"\
	"{"\
	"	float4 Pos : POSITION;"\
	"	uint4 Color : COLOR;"\
	"};"\
	""\
	"struct PS_INPUT"\
	"{"\
	"	float4 Pos : SV_POSITION;"\
	"	float4 Color : COLOR;"\
	"};"\
	""\
	"PS_INPUT VS( VS_INPUT In )"\
	"{"\
	"	PS_INPUT OUT = (PS_INPUT)0;"\
	""\
	"	OUT.Pos = mul( In.Pos, WVP );"\
	"	OUT.Pos.z = OUT.Pos.z * OUT.Pos.w;"\
	"	OUT.Color.x = In.Color.x/255.f;"\
	"	OUT.Color.y = In.Color.y/255.f;"\
	"	OUT.Color.z = In.Color.z/255.f;"\
	"	OUT.Color.w = In.Color.w/255.f;"\
	""\
	"	return OUT;"\
	"}"\
	""\
	"float4 PS( PS_INPUT In) : SV_Target"\
	"{"\
	"	return In.Color;"\
	"}";

RDX11ShaderLine::RDX11ShaderLine()
{

	SHADER_COMPILE_DESC desc;

	desc.pSrc = g_strLineEffectFile;
	desc.SrcDataSize = sizeof(g_strLineEffectFile);
	desc.szEntrypoint = "VS";
	desc.szShaderModel = "vs_4_0_level_9_1";
	desc.debugName = "LineVS";
	desc.eVertexyType = FVF_3FP_1DC;
	desc.flag = D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;

	CreateVS(desc);

	desc.szEntrypoint = "PS";
	desc.szShaderModel = "ps_4_0_level_9_1";
	desc.debugName = "LinePS";

	CreatePS(desc);


	GRAPHIC_DEVICE_DESC renderDesc;
	renderDesc.DepthStencil = DEPTH_STENCIL_ON;
	renderDesc.RasterizerState = RASTERIZER_CULL_BACK;
	renderDesc.BlendState = BLEND_NONE;

	SetRenderState( renderDesc );
}


void RDX11ShaderLine::SetShaderContants(XMMATRIX& tm)
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