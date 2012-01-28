//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer CB_Model : register( b0 )
{
	matrix m_WVP;
	matrix m_WV;	
    matrix m_World;
};

//--------------------------------------------------------------------------------------
// IO structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;	
	float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
	float3 Tangent : TANGENT;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 ViewNormal : TEXCOORD1;
    float3 ViewTangent : TEXCOORD2;
    float3 ViewBinormal : TEXCOORD3;
    float  Depth : TEXCOORD4;	
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT In )
{
    PS_INPUT OUT = (PS_INPUT)0;
	
    OUT.Pos = mul( In.Pos, m_WVP );
    OUT.Tex = In.Tex;
	OUT.ViewNormal = mul(In.Normal, m_WV);
	OUT.ViewTangent = mul(In.Tangent, m_WV);
	OUT.ViewBinormal = mul( cross(In.Normal, In.Tangent), m_WV);
    OUT.Depth = mul( In.Pos, m_WV ).z;
	
    return OUT;
}

