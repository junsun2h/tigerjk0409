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

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer CB_Light : register( b10 )
{
// sun
	float		g_lightCount;
	
// point light

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