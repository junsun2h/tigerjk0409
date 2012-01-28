//--------------------------------------------------------------------------------------
// IO structures
//--------------------------------------------------------------------------------------
cbuffer CB_Global : register( b0 )
{
	matrix g_InvProj;
	float g_FarClip;	
};

struct VS_INPUT
{
    uint Id	: SV_VertexID ;
};

struct PS_INPUT
{
    float4 Pos		: SV_Position;		//Position
	float2 Tex		: TEXCOORD0;
 	float3 ViewRay	: TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT In)
{
    PS_INPUT OUT;

    OUT.Pos.y = -1.0f + (In.Id%2) * 2.0f;
    OUT.Pos.x = -1.0f + (In.Id/2) * 2.0f;
    OUT.Pos.z = 0.5f;
    OUT.Pos.w = 1;
    OUT.Tex.x = In.Id/2;
    OUT.Tex.y = 1.0f - In.Id%2;
	
	OUT.ViewRay = mul( OUT.Pos, g_InvProj);
	OUT.ViewRay.xy *= -1;
	OUT.ViewRay = float3( OUT.ViewRay.xy * (g_FarClip/OUT.ViewRay.z), g_FarClip );	
	
	return OUT;
}