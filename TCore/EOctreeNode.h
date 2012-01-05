#pragma once


#include <vector>
#include "CDefine.h"



class EOctreeNode
{
	#define OCTREE_CHILD_NUM	8
public:
	EOctreeNode();
	~EOctreeNode();

	void	Create(CVector3 center, UINT unitLength, UINT limitUnitLegnth, UINT limitHeight);

	bool				IsEmpty()			{ return m_bEmpty; }
	const CVector3&		GetCenter()			{ return m_Center; }

private:
	EOctreeNode*	m_parent;
	EOctreeNode*	m_Child;

	CVector3		m_Center;
	UINT			m_UnitLength;
	bool			m_bEmpty;

	std::vector<long>	m_Entities;		// only leaf node has entity list
};