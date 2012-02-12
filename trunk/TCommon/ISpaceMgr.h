#pragma once

struct IEntity;

struct ISpaceMgr
{
	virtual	void		Init(UINT worldSize, UINT divideCount) = 0;
	virtual void		Destroy() =0;
	virtual void		Render() = 0;

	virtual void		UpdateVisibleSpaceList(const CCAMERA_DESC* pCameraDesc) =0;
	virtual void		UpdateEntitySpaceList(IEntity* pEntity) =0;
};