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


//--------------------------------------------------------------------------------------
// PS inputs
//--------------------------------------------------------------------------------------
struct PSIN_4P
{
    float4 Pos : SV_POSITION;
};

struct PsIn
{
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0; 
    float3 ViewNormal : TEXCOORD1;
	
#ifdef _BUMP
    float3 ViewTangent : TEXCOORD2;
    float3 ViewBinormal : TEXCOORD3;
#endif

#ifdef _LIGHT
    float3 ViewPos : TEXCOORD4;
    float3 lightViewPos : TEXCOORD5;
    float3 lightColor : TEXCOORD6;
#endif
};

struct PSIN_4P4C
{
    float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

struct PSIN_4P4C2T
{
    float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD0;
};