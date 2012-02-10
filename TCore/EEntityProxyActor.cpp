#include "EGlobal.h"
#include "IMotionInstance.h"
#include "EEntityProxyActor.h"


const CResourceActor* EEntityProxyActor::GetResource()
{
	return m_pResource;
}

void EEntityProxyActor::ProcessEvent( EntityEvent &event )
{

}

void EEntityProxyActor::Init(IEntity* pEntity)
{
	m_pEntity = pEntity;
	m_pResource = NULL;
	m_bPause = false;
}

void EEntityProxyActor::SetActor(const CResourceActor* pResource)
{
	if( m_pResource != NULL)
		Destroy();

	m_pResource = pResource;

	IEntityMgr* pEntityMgr = GLOBAL::EntityMgr();

	for( UINT i=0 ; i < pResource->jointList.size(); ++i )
	{
		const CJoint& joint = pResource->jointList[i];
		std::string name = m_pEntity->GetName() + "_" + joint.name;
		IEntity* pJointEntity = pEntityMgr->SpawnEntity( name );

		pJointEntity->SetLocalPos( joint.pos );
		pJointEntity->SetLocalRot( joint.rot );
		
		if( joint.parentIndex == -1 )
			m_pEntity->AttachChild( pJointEntity, true );
		else
			m_pJointEntities[joint.parentIndex]->AttachChild(pJointEntity, true);

		m_pJointEntities.push_back(pJointEntity);
	}
}

void EEntityProxyActor::Destroy()
{
	IEntityMgr* pEntityMgr = GLOBAL::EntityMgr();
	
	for(int i = m_pJointEntities.size() -1 ; i > -1 ; --i )
	{
		pEntityMgr->RemoveEntity( m_pJointEntities[i]->GetID() );
	}

	m_pJointEntities.clear();
	m_pResource = NULL;
}

void EEntityProxyActor::Play(CMotionDesc* pDesc)
{
	if( m_bPause )
		m_bPause = false;
	/*
	TMotionInstance* pMotionInstance = NULL;

	pMotionInstance = new TMotionInstance(pMotion, m_pMotionPose, param, m_PlayOrder);

	m_MotionList.push_back( pMotionInstance );
	sort(m_MotionList.begin(),m_MotionList.end(), SortMotionInstance);
	
	return pMotionInstance;*/
}

void EEntityProxyActor::Freeze()
{

}

void EEntityProxyActor::Stop()
{

}

bool EEntityProxyActor::IsPlaying()
{
	return false;
}

void EEntityProxyActor::UpdateJoint()
{
	UINT jointCount = m_pJointEntities.size();

	for(UINT i=0; i < jointCount; ++i)
	{
		XMMATRIX tm = XMMatrixRotationQuaternion( m_JointMatrix[i].rot.m128 );
		tm.r[3] = m_JointMatrix[i].pos.ToXMVEECTOR();

		m_pJointEntities[i]->SetLocalTM(tm);
	}
}
