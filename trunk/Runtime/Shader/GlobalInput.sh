//--------------------------------------------------------------------------------------
// vs inputs
//--------------------------------------------------------------------------------------
struct VsIn
{
	float4 Pos : POSITION;	
	float4 Normal : NORMAL;
};

struct VsIn_Bump
{
	float4 Pos : POSITION;	
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;
};

struct VsIn_Skin
{
	float4 Pos : POSITION;	
	float4 Normal : NORMAL;
    uint4 Bones : BONES;		
    uint4 Weights : WEIGHTS;
};

struct VsIn_Bump_Skin
{
	float4 Pos : POSITION;	
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;	
    uint4 Bones : BONES;		
    uint4 Weights : WEIGHTS;
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
};

struct PsIn_Bump
{
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0; 
    float3 ViewNormal : TEXCOORD1;
    float3 ViewTangent : TEXCOORD2;
    float3 ViewBinormal : TEXCOORD3;
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