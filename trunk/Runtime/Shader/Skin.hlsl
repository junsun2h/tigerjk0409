//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#define MAX_BONE_MATRICES 100

cbuffer CB_Model : register( b0 )
{
     matrix refMatrix[MAX_BONE_MATRICES];
};

//--------------------------------------------------------------------------------------
// IO structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
	float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
    uint4  Bones : BONES;		
    float3 Weights : WEIGHTS;
	float3 Tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 Pos	: POSITION;
    float3 Normal : NORMAL;
    float2 Tex	: TEXCOORD;
    float3 Tangent : TANGENT;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT In )
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;
    
	OUT.Pos = 0;
	OUT.Normal = 0;
	OUT.Tangent = 0;
	OUT.Tex = In.Tex;

	float weight4 = 1.f;
	
	for(int i=0; i <3; i++)
	{
		if( weight4 < 0.01f )
			break;
			
		OUT.Pos += In.Weights[i] * mul(In.Pos, refMatrix[ In.Bones[i] ] );
		OUT.Normal += In.Weights[i] * mul(In.Normal, refMatrix[ In.Bones[i] ] );
		OUT.Tangent += In.Weights[i] * mul(In.Tangent, refMatrix[ In.Bones[i] ] );
		weight4 -= In.Weights[i];
	}

	if( weight4 > 0.01f )
	{
		OUT.Pos += weight4 * mul(In.Pos, refMatrix[ In.Bones[3] ] );
		OUT.Normal += weight4 * mul(In.Normal, refMatrix[ In.Bones[3] ] );
		OUT.Tangent += weight4 * mul(In.Tangent, refMatrix[ In.Bones[3] ] );
	}
	
	normalize(OUT.Normal);
	normalize(OUT.Tangent);
	
    return OUT;
}

//-------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(1)] 
void GS(point VS_OUTPUT input[1], inout PointStream<VS_OUTPUT> stream) 
{	
	stream.Append(input[0]);
	stream.RestartStrip();
}