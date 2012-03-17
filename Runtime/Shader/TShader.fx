//--------------------------------------------------------------------------------------
// vs inputs
//--------------------------------------------------------------------------------------
struct VsIn
{
	float4 Pos : POSITION;	
	float4 Normal : NORMAL;
	
#ifdef _BUMP
	float4 Tangent : TANGENT;
#endif 

#ifdef _SKIN
    uint4 Bones : BONES;		
    uint4 Weights : WEIGHTS;
#endif
};


//--------------------------------------------------------------------------------------
// PS inputs
//--------------------------------------------------------------------------------------
struct PsIn
{
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0; 
    float3 ViewNormal : TEXCOORD1;
	float  Depth : TEXCOORD2;
	
#ifdef _BUMP
    float3 ViewTangent : TEXCOORD3;
    float3 ViewBinormal : TEXCOORD4;
#endif

#ifdef _LIGHT
    float3 ViewPos : TEXCOORD5;
    float3 lightViewPos : TEXCOORD6;
    float3 lightColor : TEXCOORD7;
#endif

};


//--------------------------------------------------------------------------------------
// Vertex Shader Constant Buffer Variables
//--------------------------------------------------------------------------------------
#define MAX_BONE_MATRICES 55
#define MAX_LIGHT_PER_MESH 5
#define _PI 3.14159265f

struct LightDesc
{
	float3	pos;
	float	range;
	float3	color;
	float	intensity;
};

cbuffer CB_Helper : register( b9 )
{
	float4		g_HelperMeshColor;
};

cbuffer CB_Light : register( b10 )
{
	float		g_lightCount;
	float3		dummy;
	LightDesc	g_lightDesc[MAX_LIGHT_PER_MESH];
};

cbuffer CB_Transform : register( b11 )
{
	matrix 		g_WVP;
	matrix 		g_WV;
};

cbuffer CB_SkinTransform : register( b12 )
{
     matrix 	g_aniMatrix[MAX_BONE_MATRICES];
};



//--------------------------------------------------------------------------------------
// Pixel Shader Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register( t0 );
Texture2D txNormal : register( t1 );
Texture2D txSpecular : register( t2 );
Texture2D txLight : register( t3 );
Texture2D txSSAO : register( t4 );

Texture2D txDepth : register( t5 );

SamplerState samPoint : register( s0 );
SamplerState samLinear : register( s1 );
SamplerState samAF16 : register( s2 );


cbuffer CB_Camera : register( b12 )
{
	matrix  g_viewTM;
	matrix  g_projTM;
	
	float	g_nearClip;
	float	g_farClip;
	float	g_Fovy;
	float	g_aspect;
	
	float4	g_eyePos;
	float4	g_eyeDirection;
	float4	g_upVector;
};


cbuffer CB_Sun : register( b11 )
{
	float3	g_SunDirection;
	float	g_SunIntensity;
	float3	g_ambientColor;
	float	g_ambientIntensity;	
};


#define SPECULAR_NONE 0
#define SPECULAR_PHONG 1
#define SPECULAR_MAP 2


//--------------------------------------------------------------------------------------
//  functions for vertex shader
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
		float weight = (1.0f / len) * pow( dot(vertexNormal, lightDir), 2) * falloff * g_lightDesc[i].intensity;
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

#ifdef _BUMP
    float3 bump = 2.0f * ( txNormal.Sample( samLinear, In.Tex).rgb - float3(0.5,0.5,0.5) );
	In.ViewNormal = In.ViewNormal + bump.x*In.ViewTangent + bump.y*In.ViewBinormal;
    In.ViewNormal = normalize(In.ViewNormal);
#endif

#ifdef _LIGHT
	const float _INTERTHRESHOLD = 1.0f;

	float3 PointLightVec = In.lightViewPos - In.ViewPos;
	float3 PointLightDir = length(PointLightVec) < _INTERTHRESHOLD ? PointLightVec : normalize(PointLightVec);
	float3 lightColor = In.lightColor * saturate(dot(In.ViewNormal, PointLightDir));
	float NL = saturate( dot(In.ViewNormal, PointLightDir) );

	lightColor = lightColor * NL + g_ambientColor;
	diffuse *= lightColor; 
	
#ifdef _SPECULA_MAP
/*	float3 camDir = -normalize(In.ViewPos);	
	float3 Hn = normalize(In.ViewNormal + camDir);
	float spec = pow( saturate( dot(In.ViewNormal, Hn) ), 1.0f);
	diffuse += txSpecular.Sample( samLinear, In.Tex).rgb * spec;*/
#endif

#endif //_LIGHT



	return float4(diffuse, 1);
}



//--------------------------------------------------------------------------------------
// vs inputs
//--------------------------------------------------------------------------------------

struct VSIN_4P
{
	float4 Pos : POSITION;	
};

struct VSIN_4P4C2T
{
	float4 Pos : POSITION;	
	uint4 Color : COLOR;
	float2 Tex : TEXCOORD0;
};

struct VSIN_4P4C
{
    float4 Pos : POSITION;
	uint4 Color : COLOR;
};

struct VSIN_QUAD
{
    uint Id	: SV_VertexID ;
};

//--------------------------------------------------------------------------------------
// PS inputs
//--------------------------------------------------------------------------------------
struct PSIN_4P
{
    float4 Pos : SV_POSITION;
};

struct PSIN_4P4C
{
    float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

struct PSIN_4P2T
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

struct PSIN_4P4C2T
{
    float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PSIN_4P4C VS_POS( VSIN_4P In )
{
    PSIN_4P4C OUT = (PSIN_4P4C)0;
	
	OUT.Pos.w = 1;
    OUT.Pos = mul( In.Pos, g_WVP );
	OUT.Pos.z = OUT.Pos.z * OUT.Pos.w;

	OUT.Color = g_HelperMeshColor;
	
	return OUT;
}

PSIN_4P4C VS_COLOR( VSIN_4P4C In )
{
    PSIN_4P4C OUT = (PSIN_4P4C)0;

	OUT.Pos.w = 1;
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

PSIN_4P2T VS_QUAD(VSIN_QUAD In)
{
    PSIN_4P2T OUT;

    OUT.Pos.y = -1.0f + (In.Id/2) * 2.0f;
    OUT.Pos.x = -1.0f + (In.Id%2) * 2.0f;
		
    OUT.Pos.z = 0;
    OUT.Pos.w = 1;
	
    OUT.Pos = mul( OUT.Pos, g_WVP );
	OUT.Pos.z = OUT.Pos.z * OUT.Pos.w;

    OUT.Tex.x = In.Id/2;
    OUT.Tex.y = 1.0f - In.Id%2;
	
    return OUT;
}

PSIN_4P2T VS_FULLQUAD(VSIN_QUAD In)
{
    PSIN_4P2T OUT = (PSIN_4P2T)0;

    OUT.Pos.y = -1.0f + (In.Id%2) * 2.0f;
    OUT.Pos.x = -1.0f + (In.Id/2) * 2.0f;
    OUT.Pos.z = 0.5f;
    OUT.Pos.w = 1;
	
    OUT.Tex.x = In.Id/2;
    OUT.Tex.y = 1.0f - In.Id%2;
	
    return OUT;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_FONT( PSIN_4P4C2T In) : SV_Target
{
    return txDiffuse.Sample( samAF16, In.Tex ) * In.Color;
}

float4 PS_COLOR( PSIN_4P4C In) : SV_Target
{
    return In.Color;
}

float4 PS_TEXTURE( PSIN_4P2T In) : SV_Target
{
    return txDiffuse.Sample( samLinear, In.Tex );
}