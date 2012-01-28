#pragma once

#include "ISpaceMgr.h"
#include "CUnitPool.h"
#include "EQuadSpaceTreeNode.h"
#include <atlcoll.h>

class EEntity;
struct IEntityProxyCamera;

class EQuadSpaceTreeMgr : public ISpaceMgr
{
public:
	EQuadSpaceTreeMgr();
	~EQuadSpaceTreeMgr();

	void				Init(UINT worldSize, UINT divideCount);
	void				Destroy();

	void				UpdateVisibleSpaceList(IEntityProxyCamera* pCamera);
	void				UpdateEntitySpace(IEntity* pEntity) override;

	void				Render() override;

private:
	EQuadSpaceTreeNode*	GetSpace(float x, float y);
	EQuadSpaceTreeNode*	GetSpace(UINT id);
	void				AssignSpace(EEntity* pEntity);

	typedef	ATL::CAtlMap<UINT, EQuadSpaceTreeNode*>	TYPE_SPACE_NODE_MAP;
	typedef	std::vector<EQuadSpaceTreeNode*>		TYPE_VISIBLE_SPACE_LIST;

	CObjectPool<EQuadSpaceTreeNode>		m_SpaceNodePool;
	
	TYPE_SPACE_NODE_MAP					m_SpaceMap;
	TYPE_VISIBLE_SPACE_LIST				m_VisibleSpaceList;

	UINT								m_WorldSize;
	int									m_WorldStart;
	UINT								m_WorldUnitLength;

	int									m_DivideCount;
};