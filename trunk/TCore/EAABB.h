#pragma once

#include "IAABB.h"

class EAABB : public IAABB
{
public:
	EAABB();
	void		AddAABB(XMMATRIX& worldTM, CVector3& min, CVector3& max) override;
	void		AddAABB(IAABB* pAABB) override;
	void		GetAABBPoints(CVector3 vPoints[], const XMMATRIX& tm) override;
	void		Reset() override;
	bool		IsValid() const override;

	CVector3	GetMax() const override		{ return m_Max; }
	CVector3	GetMin() const override		{ return m_Min; }
	CVector3	GetCenter() const override	{ return ( m_Min + m_Max ) * 0.5f; }
	CVector3	GetExtent() const override	{ return ( m_Max - m_Min ) * 0.5f; }
	
protected:
	bool		m_IsValid;

	CVector3	m_Min;
	CVector3	m_Max;
};