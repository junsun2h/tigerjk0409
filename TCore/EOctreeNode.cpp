#include "EOctreeNode.h"


EOctreeNode::EOctreeNode()
	: m_Child(NULL)
	, m_bEmpty(true)
{
}

EOctreeNode::~EOctreeNode()
{

}

void EOctreeNode::Create(CVector3 center, UINT unitLength, UINT limitUnitLegnth, UINT limitHeight)
{
	assert( unitLength > limitUnitLegnth );

	m_Center = center;
	m_UnitLength = limitUnitLegnth;

	float quartUnit = float(m_UnitLength/4);
	UINT childUnitLength = m_UnitLength/2;

	CVector3 childCenters[8];

	childCenters[0] = center + CVector3( -quartUnit, -quartUnit, quartUnit);
	childCenters[1] = center + CVector3( -quartUnit, +quartUnit, quartUnit);
	childCenters[2] = center + CVector3( quartUnit, quartUnit, quartUnit);
	childCenters[3] = center + CVector3( quartUnit, -quartUnit, quartUnit);

	childCenters[0] = center + CVector3( -quartUnit, -quartUnit, -quartUnit);
	childCenters[1] = center + CVector3( -quartUnit, +quartUnit, -quartUnit);
	childCenters[2] = center + CVector3( quartUnit, quartUnit, -quartUnit);
	childCenters[3] = center + CVector3( quartUnit, -quartUnit, -quartUnit);
		
	if( childUnitLength > limitUnitLegnth )
	{
		m_Child = new EOctreeNode[OCTREE_CHILD_NUM];
		
		for( int i=0 ; i < OCTREE_CHILD_NUM; ++i)
		{
			if( childCenters[i].z > limitHeight )
				continue;

			m_Child->Create(childCenters[i], childUnitLength, limitUnitLegnth, limitHeight);
		}
	}
}
