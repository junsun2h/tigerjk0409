//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer CB_Model : register( b0 )
{
	matrix m_WVP;
};

//--------------------------------------------------------------------------------------
// IO structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;	
	float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos	: SV_POSITION;
    float2 Tex : TEXCOORD0;	
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT In )
{
    PS_INPUT OUT = (PS_INPUT)0;
		
	OUT.Pos = mul( In.Pos, m_WVP );
	OUT.Tex = In.Tex;

	return OUT;
}