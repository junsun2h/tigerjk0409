#pragma once

struct IEntity;

struct ISpaceMgr
{
	virtual void		Render() = 0;
	virtual void		UpdateEntitySpace(IEntity* pEntity) =0;
};