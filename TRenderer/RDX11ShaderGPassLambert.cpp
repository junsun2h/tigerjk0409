#include "RDefine.h"

#include "CResource.h"
#include "CCamera.h"

#include "IRDX11Device.h"
#include "IEntityProxy.h"
#include "IRenderStateMgr.h"
#include "IShader.h"


#include "RDX11Global.h"
#include "RDX11Shader.h"
#include "RDX11ShaderGPassLambert.h"


RDX11ShaderGPassLambert::RDX11ShaderGPassLambert()
{
	SHADER_COMPILE_DESC desc;

	desc.szFileName = "Shader\\GPassLambert.vs";
	desc.szEntrypoint = "VS";
	desc.szShaderModel = "vs_4_0";
	desc.eVertexyType = FVF_4HP_4BN_2HT;

	CreateVS(desc);

	desc.szEntrypoint = "VS_SKIN";
	desc.eVertexyType = FVF_4HP_4BN_2HT_4BW;

//	CreateVS(desc);

	desc.szFileName = "Shader\\GPassLambert.ps";
	desc.szEntrypoint = "PS";
	desc.szShaderModel = "ps_4_0";

	CreatePS(desc);

	GRAPHIC_DEVICE_DESC renderDesc;
	renderDesc.DepthStencil = DEPTH_STENCIL_WRITE;
	renderDesc.RasterizerState = RASTERIZER_CULL_BACK;
	renderDesc.BlendState = BLEND_NONE;

	SetRenderState( renderDesc );
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