//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer CB_Model : register( b11 )
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
	int4 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0; 
    float3 ViewNormal : TEXCOORD1;
    float Depth : TEXCOORD2;	
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT In )
{
    PS_INPUT OUT = (PS_INPUT)0;
	
	float4 normal = In.Normal/127.f;

    OUT.Pos = mul( In.Pos, m_WVP );
	OUT.Tex = In.Tex;
	OUT.ViewNormal = normal;//mul(normal, m_WV);
    OUT.Depth = mul( In.Pos, m_WV ).z ;
	
    return OUT;
}