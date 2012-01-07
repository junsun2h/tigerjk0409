#pragma once


char g_strLineEffectFile[] = \
	"cbuffer CB_Model : register( b6 )"\
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
	"              float4 Dif : COLOR,"\
	"              float2 vTexCoord0 : TEXCOORD )"\
	"{"\
	"    VS_OUTPUT Output;"\
	""\
	"    Output.Pos = float4( vPos, 1.0f );"\
	"    Output.Dif = Dif;"\
	"    Output.Tex = vTexCoord0;"\
	""\
	"    return Output;"\
	"}"\
	""\
	"float4 PS( VS_OUTPUT In ) : SV_Target"\
	"{"\
	"    return txDiffuse.Sample( samAF16, In.Tex ) * In.Dif;"\
	"}";
