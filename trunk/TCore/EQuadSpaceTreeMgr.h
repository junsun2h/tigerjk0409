#pragma once

typedef	ATL::CAtlMap<UINT, EQuadSpaceTreeNode*>	TYPE_SPACE_NODE_MAP;
typedef	std::vector<EQuadSpaceTreeNode*>		TYPE_VISIBLE_SPACE_LIST;


class EQuadSpaceTreeMgr : public ISpaceMgr
{
public:
	EQuadSpaceTreeMgr();
	~EQuadSpaceTreeMgr();

	void				Init(UINT worldSize, UINT divideCount);

private:
	void				Destroy() override;
	void				UpdateVisibleSpaceList(const CCAMERA_DESC* pCameraDesc) override;
	void				UpdateEntitySpaceList(IEntity* pEntity) override;
	void				UpdateEntityCull(IEntity* pEntity);

	void				Render() override;

	EQuadSpaceTreeNode*	GetSpace(float x, float y);
	EQuadSpaceTreeNode*	GetSpace(UINT id);
	void				RegistEntityToSpaceNode(IEntity* pEntity);

	CObjectPool<EQuadSpaceTreeNode>		m_SpaceNodePool;
	
	TYPE_SPACE_NODE_MAP					m_SpaceMap;
	TYPE_VISIBLE_SPACE_LIST				m_VisibleSpaceList;

	UINT								m_WorldSize;
	int									m_WorldStart;
	UINT								m_WorldUnitLength;

	int									m_DivideCount;
};