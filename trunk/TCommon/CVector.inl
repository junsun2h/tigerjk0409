inline CVector2	CVector2::Normalize(const CVector2& v)							
{
	CVector2 out = v;
	float length = Length(v); 

	if( length == 0)
		return out;

	out.x /= length; 
	out.y /= length;
	return out;
}

inline CVector3	CVector3::Transform(const CVector3& v, const CMatrix& mt )			
{
	return XMVector3Transform( v.ToXMVEECTOR(), mt._m); 
}

inline CVector3	CVector3::TransformCoord(const CVector3& v, const CMatrix& mt )		
{ 
	return XMVector3TransformCoord( v.ToXMVEECTOR(), mt._m); 
}

inline CVector3	CVector3::TransformNormal(const CVector3& v, const CMatrix& mt )			
{
	return XMVector3TransformNormal( v.ToXMVEECTOR(), mt._m); 
}

