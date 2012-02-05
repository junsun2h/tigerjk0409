#pragma once


class EAABB : public IAABB
{
public:
	EAABB();
	void		AddAABB(XMMATRIX& worldTM, CVector3& min, CVector3& max) override;
	void		AddAABB(CVector3& min, CVector3& max ) override;
	void		AddAABB(const IAABB* pAABB) override;
	void		GetAABBPoints(CVector3 vPoints[], const XMMATRIX& tm) override;
	void		Reset() override;

	bool		IsValid() const override;
	bool		IsPointInBox(const CVector3 &InP, XMMATRIX* pWorld = NULL ) const override;
	bool		IsSphereInBox( const CVector3 &InP, float fRadius, XMMATRIX* pWorld = NULL ) const override;
	bool		IsLineInBox( const CVector3& L1, const CVector3& L2, XMMATRIX* pWorld = NULL  ) const override;

	CVector3	GetMax() const override		{ return m_Max; }
	CVector3	GetMin() const override		{ return m_Min; }
	CVector3	GetCenter() const override	{ return ( m_Min + m_Max ) * 0.5f; }
	CVector3	GetExtent() const override	{ return ( m_Max - m_Min ) * 0.5f; }
	
	BOOL operator == ( const EAABB& aabb) const	
	{
		if( m_IsValid == false && aabb.m_IsValid == false )
			return true;

		if( m_Min != aabb.m_Min || m_Max != aabb.m_Max )
			return false;

		return true;
	}

	BOOL operator != ( const EAABB& aabb) const	
	{
		return !(*this == aabb );
	}

protected:
	XMMATRIX	CalculateAABBCoordinate() const;

	bool		m_IsValid;
	CVector3	m_Min;
	CVector3	m_Max;
};