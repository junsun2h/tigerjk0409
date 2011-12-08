inline CVector3	CMatrix::GetRow(int row) const
{
	return CVector3( _m.m[row][0], _m.m[row][1], _m.m[row][2]);
}

inline void	CMatrix::SetRow(const CVector3& vec, int row)
{
	_m.m[row][0] = vec.x;
	_m.m[row][1] = vec.y;
	_m.m[row][2] = vec.z;
}

inline BOOL	CMatrix::IsValid(const CMatrix& mt)		
{
	return !XMMatrixIsInfinite(mt._m); 
}

inline BOOL CMatrix::IsIdentity(const CMatrix& mt)	
{ 
	return XMMatrixIsIdentity(mt._m); 
}

inline XMMATRIX	CMatrix::Multiply(const CMatrix& mt1, const CMatrix& mt2)			
{ 
	return XMMatrixMultiply(mt1._m, mt2._m); 
}

inline XMMATRIX	CMatrix::MultiplyTranspose(const CMatrix& mt1, const CMatrix& mt2)	
{ 
	return XMMatrixMultiplyTranspose(mt1._m, mt2._m); 
}

inline XMMATRIX	CMatrix::Transpose(const CMatrix& mt)	
{ 
	return  XMMatrixTranspose(mt._m); 
}
					
inline XMVECTOR	CMatrix::Determinant(const CMatrix& mt)						
{ 
	return XMMatrixDeterminant(mt._m); 
};

inline XMMATRIX	CMatrix::Inverse(CVector3* pDeterminant, const CMatrix& mt)		
{  
	XMVECTOR _m128;
	XMMATRIX inv = XMMatrixInverse( &_m128, mt._m);
	
	if( pDeterminant != NULL)
		*pDeterminant = _m128;

	return inv;
}

inline BOOL CMatrix::Decompose(CVector3* pScale, CQuat* pRot, CVector3* pPos, const CMatrix& mt)
{
	XMVECTOR outScale;
	XMVECTOR outRotQuat;
	XMVECTOR outTrans;

	if( !XMMatrixDecompose( &outScale, &outRotQuat, &outTrans, mt._m) )
		return false;

	*pScale = outScale;
	*pRot = outRotQuat;
	*pPos = outTrans;

	return true;
}

inline void CMatrix::ToRollPitchYaw(float& pitch, float& yaw, float& roll, const CMatrix& mt)
{
	//	since xna math rotate Roll(Z) -> Pitch(X) -> Yaw(Y), we have to decompose from Yaw, followed by Pitch and finally roll
	CVector3 y = mt.GetRow(1);
	CVector3 z = mt.GetRow(2);

	// decompose y-aix( yaw )
	yaw = XMConvertToDegrees( atan2f( z.x, z.z ) );
		
	// decompose x-aix( pitch )
	CMatrix rot_y = CMatrix::RotateY( XMConvertToRadians( -yaw)  );
		
	y = CVector3::Transform( y , rot_y );
	z = CVector3::Transform( z , rot_y );

	pitch = XMConvertToDegrees( - atan2f( z.y, z.z ) );

	// decompose z-aix( roll )
	CMatrix rot_z = CMatrix::RotateX( XMConvertToRadians( -pitch)  );

	y = CVector3::Transform( y , rot_z );

	roll = XMConvertToDegrees( -atan2f( y.x, y.y ) );
}



