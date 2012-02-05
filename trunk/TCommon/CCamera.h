#pragma once


struct CCAMERA_DESC
{
	XMMATRIX ViewTM;
	XMMATRIX ProjTM;

	float	nearClip;
	float	farClip;
	float	Fovy;
	float	aspect;

	CVector4	eyePos;
	CVector4	eyeDirection;
	CVector4	upVector;
};
