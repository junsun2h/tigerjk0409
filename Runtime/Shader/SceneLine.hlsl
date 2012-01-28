//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

cbuffer CB_Model : register( b0 )
{
	matrix WVP;	
};

//--------------------------------------------------------------------------------------

struct VS_INPUT
{
    float4 Pos : POSITION;
	uint4 Color : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT In )
{
    PS_INPUT OUT = (PS_INPUT)0;
		
    OUT.Pos = mul( In.Pos, WVP );
	OUT.Color.x = In.Color.x/255.f;
	OUT.Color.y = In.Color.y/255.f;
	OUT.Color.z = In.Color.z/255.f;
	OUT.Color.w = In.Color.w/255.f;	
	
    return OUT;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT In) : SV_Target
{
    return In.Color;
}
