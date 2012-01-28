#include "RDX11ShaderFont.h"

char g_strFontFxFile[] = \
	"texture2D txDiffuse : register( t0 );"\
	""\
	"SamplerState samAF16 : register( s2 );"\
	""\
	"struct VS_OUTPUT"\
	"{"\
	"    float4 Pos : POSITION;"\
	"    float4 Dif : COLOR;"\
	"    float2 Tex : TEXCOORD;"\
	"};"\
	""\
	"VS_OUTPUT VS( float3 vPos : POSITION,"\
	"              uint4 Color : COLOR,"\
	"              float2 vTexCoord0 : TEXCOORD )"\
	"{"\
	"    VS_OUTPUT Output;"\
	""\
	"	Output.Pos = float4( vPos, 1.0f );"\
	"	Output.Dif.x = Color.x/255.f;"\
	"	Output.Dif.y = Color.y/255.f;"\
	"	Output.Dif.z = Color.z/255.f;"\
	"	Output.Dif.w = Color.w/255.f;"\
	"	Output.Tex = vTexCoord0;"\
	""\
	"    return Output;"\
	"}"\
	""\
	"float4 PS( VS_OUTPUT In ) : SV_Target"\
	"{"\
	"    return txDiffuse.Sample( samAF16, In.Tex ) * In.Dif;"\
	"}";



RDX11ShaderFont::RDX11ShaderFont()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R16G16_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	SHADER_COMPILE_DESC desc;

	desc.pSrc = g_strFontFxFile;
	desc.SrcDataSize = sizeof(g_strFontFxFile);
	desc.szEntrypoint = "VS";
	desc.szShaderModel = "vs_4_0_level_9_1";
	desc.pLayout = layout;
	desc.layoutSize = ARRAYSIZE( layout );
	desc.debugName = "FontVS";
	desc.flag = D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;

	CreateVS(desc);

	desc.szEntrypoint = "PS";
	desc.szShaderModel = "ps_4_0_level_9_1";
	desc.debugName = "FontPS";

	CreatePS(desc);

	SetRenderState(DEPTH_STENCIL_OFF, RASTERIZER_CULL_BACK, BLEND_ADD_BY_ALPHA);
	SetTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}
