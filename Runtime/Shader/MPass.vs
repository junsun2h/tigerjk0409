#include "GlobalInput.sh"

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer CB_Model : register( b11 )
{
	matrix m_WVP;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PSIN_4P4C VS_COLOR( VSIN_4P4C In )
{
    PSIN_4P4C OUT = (PSIN_4P4C)0;
	
    OUT.Pos = mul( In.Pos, m_WVP );
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