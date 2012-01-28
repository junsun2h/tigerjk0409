//--------------------------------------------------------------------------------------
// IO structures
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_Position;
    float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( uint id : SV_VertexID )
{
	PS_INPUT OUT = (PS_INPUT)0;
	
    float2 tex = float2( (id << 1) & 2, id & 2 );
    tex = tex * float2( 2.0f, -2.0f ) + float2( -1.0f, 1.0f);
    OUT.Pos = float4( tex, 0.0f, 1.0f );
    
    // Bottom left pixel is (0,0) and bottom right is (1,1)
    OUT.Tex.x = (id << 1) & 2;
	OUT.Tex.y = id & 2;
   
    return OUT;
}
