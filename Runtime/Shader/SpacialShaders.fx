#include "GlobalInput.sh"
#include "GlobalVSConstant.sh"
#include "GlobalPSConstant.sh"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PSIN_4P4C VS_COLOR( VSIN_4P4C In )
{
    PSIN_4P4C OUT = (PSIN_4P4C)0;
	
    OUT.Pos = mul( In.Pos, g_WVP );
	OUT.Pos.z = OUT.Pos.z * OUT.Pos.w;

	OUT.Color = In.Color/255.f;
    
	return OUT;
}

PSIN_4P4C2T VS_FONT( VSIN_4P4C2T In )
{
    PSIN_4P4C2T OUT = (PSIN_4P4C2T)0;
	
    OUT.Pos = In.Pos;
	OUT.Color = In.Color/255.f;
	OUT.Tex = In.Tex;
	
	return OUT;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_COLOR( PSIN_4P4C In) : SV_Target
{
    return In.Color;
}

float4 PS_FONT( PSIN_4P4C2T In) : SV_Target
{
    return txDiffuse.Sample( samAF16, In.Tex ) * In.Color;
}