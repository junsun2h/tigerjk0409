#pragma once

#include "CDefine.h"


struct IAABB
{
	virtual void		AddAABB(XMMATRIX& worldTM, CVector3& min, CVector3& max) = 0;
	virtual void		AddAABB(IAABB* pAABB) = 0;
	virtual void		GetAABBPoints(CVector3 vPoints[], const XMMATRIX& tm) = 0;
	virtual void		Reset() = 0;

	virtual CVector3	GetMax() const = 0;
	virtual CVector3	GetMin() const = 0;
	virtual CVector3	GetCenter() const = 0;
	virtual CVector3	GetExtent() const = 0;
	virtual bool		IsValid() const = 0;
};