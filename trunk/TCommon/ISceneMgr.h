#pragma once

#include "CDefine.h"
struct IEntityProxyCamera;


struct ISceneMgr
{
	virtual void InitOctreeSpace( UINT size, UINT limitUnitLegnth, UINT limitHeight ) = 0;
	virtual void UpdateVisibleOctreeNodeList(IEntityProxyCamera* pCamera) = 0;
	virtual void UpdateRenderObjectList() = 0;
};