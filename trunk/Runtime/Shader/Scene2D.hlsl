//--------------------------------------------------------------------------------------
// Resource Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer CB_Model : register( b0 )
{
	matrix WVP;	
};

///--------------------------------------------------------------------------------------
// IO structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    uint Id	: SV_VertexID ;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;	
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT In )
{
    PS_INPUT OUT = (PS_INPUT)0;

    OUT.Pos.y = -1.0f + (In.Id%2) * 2.0f;
    OUT.Pos.x = -1.0f + (In.Id/2) * 2.0f;
    OUT.Pos.z = 0.0f;
    OUT.Pos.w = 1;
    OUT.Tex.x = In.Id/2;
    OUT.Tex.y = 1.0f - In.Id%2;		
	
    OUT.Pos = mul( OUT.Pos, WVP );

    return OUT;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT In) : SV_Target
{
    return txDiffuse.Sample( samLinear, In.Tex );
}
