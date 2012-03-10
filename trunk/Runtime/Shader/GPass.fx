#include "GlobalInput.sh"
#include "GlobalPSConstant.sh"
#include "GlobalVSConstant.sh"

//--------------------------------------------------------------------------------------
// vertex shader functions
//--------------------------------------------------------------------------------------
#ifdef _BUMP
void ProcessWeight(inout float4 pos, inout float3 normal, inout float3 tangent, in uint4 Bones, in uint4 Weights)
#else
void ProcessWeight(inout float4 pos, inout float3 normal, in uint4 Bones, in uint4 Weights)
#endif	
{
	float4 worldPos = 0;
	float3 worldNormal = 0;
#ifdef _BUMP
	float3 worldTangent = 0;
#endif	
	
	for(int i=0; i <4; i++)
	{
		if( Weights[i] == 0 )
			break;

		float weight = Weights[i]/255.f;
		worldPos += weight * mul(  pos, g_aniMatrix[ Bones[i] ] );
		worldNormal += weight * mul( normal, g_aniMatrix[ Bones[i] ] );
#ifdef _BUMP
		worldTangent += weight * mul( tangent, g_aniMatrix[ Bones[i] ] );
#endif	
	}
	
	pos = worldPos;
	normal = normalize(worldNormal);
#ifdef _BUMP
	tangent = normalize(worldTangent);
#endif		
}



void ProcessLight(in float3 vertexPos, in float3 vertexNormal, inout float3 AvgLightPos, inout float3 AvgLightColor)
{
	float totalWeight = 0.0f;
	float3 totalLightVector = 0.0f;

	int i;
	for(i=0; i< MAX_LIGHT_PER_MESH; i++)
	{	
		if( g_lightCount == i )
			break;
			
		const float3 lightVector = g_lightDesc[i].pos - vertexPos;
		const float3 lightDir = normalize(lightVector);
		const float len = length(lightVector);

		float falloff = 0.25f * pow( cos( max(1, g_lightDesc[i].range / len) * _PI )+1, 2);
		float weight = (1.0f / len) * saturate(dot(vertexNormal, lightDir)) * falloff * g_lightDesc[i].intensity;
		totalLightVector += weight * lightDir;
		totalWeight += weight/len;
	}

	if( totalWeight <= 0)
		return;
		
	AvgLightPos = (totalLightVector / totalWeight) + vertexPos;
	
	float3 AvgLightDir = normalize( AvgLightPos - vertexPos );
	
	for(i=0; i< MAX_LIGHT_PER_MESH; i++)
	{
		if( g_lightCount == i )
			break;
	
		float3 lightVector = normalize( g_lightDesc[i].pos - vertexPos );
		AvgLightColor += saturate(dot(lightVector, AvgLightDir)) * g_lightDesc[i].color * g_lightDesc[i].intensity;
	}
}


//--------------------------------------------------------------------------------------
// Vertex Shader of Opaque object
//--------------------------------------------------------------------------------------

PsIn VS( VsIn In ){
	PsIn OUT = (PsIn)0;

	OUT.Tex.x = In.Pos.w;
	OUT.Tex.y = In.Normal.w;
	In.Pos.w = 1;
	
	float3 normal = In.Normal;
#ifdef _BUMP
	float3 tangent = In.Tangent;
#endif

#ifdef _SKIN
#ifdef _BUMP
	ProcessWeight( In.Pos, normal, tangent, In.Bones, In.Weights);
#else
	ProcessWeight( In.Pos, normal, In.Bones, In.Weights);
#endif
#endif
	
	OUT.ViewNormal = mul( normal, g_WV);
	
#ifdef _LIGHT	
	OUT.ViewPos = mul( In.Pos, g_WV);
	ProcessLight( OUT.ViewPos, OUT.ViewNormal, OUT.lightViewPos, OUT.lightColor);
#endif

	OUT.Pos = mul( In.Pos, g_WVP );
	OUT.Pos.z = OUT.Pos.z * OUT.Pos.w;

#ifdef _COLOR_VERTEX
	OUT.Color = In.Color/255.f;
#endif

#ifdef _BUMP	
	OUT.ViewTangent = mul(tangent, g_WV);
	OUT.ViewBinormal = mul( cross(OUT.ViewNormal, OUT.ViewTangent), g_WV);
#endif

    return OUT;
}


//--------------------------------------------------------------------------------------
// Pixel Shader of Opaque object
//--------------------------------------------------------------------------------------
float4 PS( PsIn In) : SV_Target
{
	float3 diffuse = txDiffuse.Sample( samLinear, In.Tex).rgb;
	float3 lightColor = 0;
	
#ifdef _BUMP
    float3 bump = 1.5f * ( txNormal.Sample( samLinear, In.Tex).rgb - float3(0.5,0.5,0.5) );
	In.ViewNormal = In.ViewNormal + bump.x*In.ViewTangent + bump.y*In.ViewBinormal;
    In.ViewNormal = normalize(In.ViewNormal);
#endif

#ifdef _LIGHT
	const float _INTERTHRESHOLD = 1.0f;

	float3 PointLightVec = In.lightViewPos - In.ViewPos;
	float3 PointLightDir = length(PointLightVec) < _INTERTHRESHOLD ? PointLightVec : normalize(PointLightVec);

	float3 PointLightColor = In.lightColor * saturate(dot(In.ViewNormal, PointLightDir));
	lightColor += PointLightColor;

	float NL = saturate( dot(In.ViewNormal, PointLightDir) ); 
#else
	float NL = saturate( dot(In.ViewNormal, g_SunDirection) ); 
#endif

	lightColor = lightColor * NL + g_ambientColor;

	float3 finalColor = diffuse * lightColor; 
	return float4(finalColor, 1);
}