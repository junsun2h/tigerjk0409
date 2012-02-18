#include "GlobalInput.sh"
#include "GlobalVSConstant.sh"

//--------------------------------------------------------------------------------------
// functions
//--------------------------------------------------------------------------------------
void ProcessWeight(inout float4 pos, inout float4 normal, in uint4 Bones, in uint4 Weights)
{
	float4 worldPos = 0;
	float4 worldNormal = 0;
	
	for(int i=0; i <4; i++)
	{
		if( Weights[i] < 10 )
			continue;
			
		float weight = Weights[i]/255.f;
		worldPos += weight * mul(  pos, g_aniMatrix[ Bones[i] ] );
		worldNormal += weight * mul( normal, g_aniMatrix[ Bones[i] ] );
	}
	
	pos = worldPos;
	normal = normalize(worldNormal);	
}



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------


PSIN_4P2T3N VS( VSIN_4P4N2T In )
{
    PSIN_4P2T3N OUT = (PSIN_4P2T3N)0;

	float4 normal = In.Normal/127.f;
	normalize(normal);
	
	//------------------------------------------------
	// set ps input
    OUT.Pos = mul( In.Pos, m_WVP );
	OUT.Pos.z = OUT.Pos.z * OUT.Pos.w;
	OUT.ViewNormal = mul( normal, m_WV);
	OUT.Tex = In.Tex;
	
    return OUT;
}

//--------------------------------------------------------------------------------------
PSIN_4P2T3N VS_SKIN( VSIN_4P4N2T4W In )
{
    PSIN_4P2T3N OUT = (PSIN_4P2T3N)0;
    
	float4 normal = In.Normal/127.f;
	normalize(normal);

	ProcessWeight( In.Pos, normal, In.Bones, In.Weights);

	//------------------------------------------------
	// set ps input
    OUT.Pos = mul( In.Pos, m_WVP );
	OUT.Pos.z = OUT.Pos.z * OUT.Pos.w;
	OUT.ViewNormal = mul( normal, m_WV);
	OUT.Tex = In.Tex;
		
    return OUT;
}