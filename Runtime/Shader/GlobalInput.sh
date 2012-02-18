//--------------------------------------------------------------------------------------
// vs inputs
//--------------------------------------------------------------------------------------
struct VSIN_4P4N2T
{
	float4 Pos : POSITION;	
	int4 Normal : NORMAL;
	float2 Tex : TEXCOORD0;
};

struct VSIN_4P4C2T
{
	float4 Pos : POSITION;	
	uint4 Color : COLOR;
	float2 Tex : TEXCOORD0;
};

struct VSIN_4P4N2T4W
{
	float4 Pos : POSITION;	
	int4 Normal : NORMAL;
	float2 Tex : TEXCOORD0;
    uint4 Bones : BONES;		
    uint4 Weights : WEIGHTS;
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

struct PSIN_4P2T3N
{
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0; 
    float3 ViewNormal : TEXCOORD1;
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