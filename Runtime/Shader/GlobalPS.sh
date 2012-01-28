Texture2D txDiffuse : register( t0 );
Texture2D txNormal : register( t1 );

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


