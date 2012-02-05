#pragma once

struct IEntity;

struct ISpaceMgr
{
	virtual	void		Init(UINT worldSize, UINT divideCount) = 0;
	virtual void		Destroy() =0;
	virtual void		Render() = 0;

	virtual void		UpdateVisibleSpaceList(IEntityProxyCamera* pCamera) =0;
	virtual void		UpdateEntitySpace(IEntity* pEntity) =0;
};