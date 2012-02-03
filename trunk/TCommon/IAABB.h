#pragma once

#include "CDefine.h"


struct IAABB
{
	virtual void		AddAABB(XMMATRIX& worldTM, CVector3& min, CVector3& max) = 0;
	virtual void		AddAABB(CVector3& min, CVector3& max ) = 0;
	virtual void		AddAABB(const IAABB* pAABB) = 0;
	virtual void		GetAABBPoints(CVector3 vPoints[], const XMMATRIX& tm) = 0;
	virtual void		Reset() = 0;

	virtual CVector3	GetMax() const = 0;
	virtual CVector3	GetMin() const = 0;
	virtual CVector3	GetCenter() const = 0;
	virtual CVector3	GetExtent() const = 0;

	virtual bool		IsValid() const = 0;
	virtual bool		IsPointInBox(const CVector3 &InP, XMMATRIX* pWorld = NULL ) const = 0;
	virtual bool		IsSphereInBox( const CVector3 &InP, float fRadius, XMMATRIX* pWorld = NULL ) const = 0;
	virtual bool		IsLineInBox( const CVector3& L1, const CVector3& L2, XMMATRIX* pWorld = NULL  ) const = 0;
};