#pragma once

#include "ISceneMgr.h"

#include <vector>
#include "EOctreeNode.h"
#include "EEntity.h"

class ESceneMgr : public ISceneMgr
{
public:
	ESceneMgr();

	void	InitOctreeSpace( UINT size, UINT limitUnitLegnth, UINT limitHeight ) override;
	void	UpdateVisibleOctreeNodeList(IEntityProxyCamera* pCamera) override;
	void	UpdateRenderObjectList() override;

private:
	typedef	std::vector<EOctreeNode*> EOCTREE_NODE_LIST;
	
	EOCTREE_NODE_LIST	m_VisibleNodes;
	EOctreeNode			m_RootSpaceNode;
	IEntity*			m_pRootEntity;
};