#define MAX_BONE_MATRICES 55

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer CB_Model : register( b11 )
{
	matrix m_WVP;
	matrix m_WV;
};

cbuffer CB_Model : register( b12 )
{
     matrix g_aniMatrix[MAX_BONE_MATRICES];
};