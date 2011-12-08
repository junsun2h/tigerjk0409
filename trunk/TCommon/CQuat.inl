inline XMVECTOR	CQuat::Conjugate(const CQuat& q)
{ 
	return XMQuaternionConjugate(q.m128);  
}

inline XMVECTOR	CQuat::Normalise(const CQuat& q)		
{ 
	return XMQuaternionNormalize(q.m128);  
}
					
inline XMVECTOR	CQuat::Ratate(const CVector3& axis, float angle)	
{ 
	return XMQuaternionRotationAxis( XMVectorSet(axis.x, axis.y, axis.z, 0), angle);  
}

// in radians, Roll(Z) -> Pitch(X) -> Yaw(Y) order
inline XMVECTOR	CQuat::RatateRadian(float Pitch, float Yaw, float Roll)	
{ 
	return XMQuaternionRotationRollPitchYaw(Pitch, Yaw, Roll);  
}

inline XMVECTOR	CQuat::RatateDegree(float Pitch, float Yaw, float Roll)	
{ 
	return RatateRadian( XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));  
}
				
inline XMVECTOR	CQuat::Slerp(const CQuat& q1, const CQuat& q2, float t)	
{ 
	return XMQuaternionSlerp( q1.m128, q2.m128, t);  
}

inline XMVECTOR	CQuat::GetDotProduct(const CQuat& q1, const CQuat& q2)		
{ 
	return XMQuaternionDot( q1.m128, q2.m128);  
}

inline XMVECTOR	CQuat::GetInverse(const CQuat& q)					
{ 
	return XMQuaternionInverse( q.m128);  
}
					
inline void	CQuat::ToAxisAngle(const CQuat& q1, CVector3* pAxis, float* pAnlge)
{
	XMVECTOR axis;
	XMQuaternionToAxisAngle(&axis, pAnlge, q1.m128);
	*pAxis = axis;
}