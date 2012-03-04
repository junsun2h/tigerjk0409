#include "GlobalInput.sh"
#include "GlobalVSConstant.sh"

//--------------------------------------------------------------------------------------
// functions
//--------------------------------------------------------------------------------------
void ProcessWeight(inout float4 pos, inout float3 normal, in uint4 Bones, in uint4 Weights)
{
	float4 worldPos = 0;
	float3 worldNormal = 0;
	
	for(int i=0; i <4; i++)
	{
		if( Weights[i] == 0 )
			break;

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
#ifdef _BUMP

#ifdef _SKIN
PsIn_Bump VS( VsIn_Bump_Skin In )
#else
PsIn_Bump VS( VsIn_Bump In )
#endif
{	
	PsIn_Bump OUT = (PsIn_Bump)0;
	
#else

#ifdef _SKIN
PsIn VS( VsIn_Skin In )
#else
PsIn VS( VsIn In )
#endif
{	
	PsIn OUT = (PsIn)0;

#endif

	float3 normal = In.Normal/127.f;
	normal = normalize(normal);
	
#ifdef _SKIN
	ProcessWeight( In.Pos, normal, In.Bones, In.Weights);
#endif

	OUT.Pos = mul( In.Pos, m_WVP );
	OUT.Pos.z = OUT.Pos.z * OUT.Pos.w;
	OUT.ViewNormal = mul( normal, m_WV);
	OUT.Tex = In.Tex;

#ifdef _BUMP
	float3 tangent = In.Tangent/127.f;
	tangent = normalize(tangent);
	
	OUT.ViewTangent = mul(tangent, m_WV);
	OUT.ViewBinormal = mul( cross(OUT.ViewNormal, OUT.ViewTangent), m_WV);
#endif

    return OUT;
}
