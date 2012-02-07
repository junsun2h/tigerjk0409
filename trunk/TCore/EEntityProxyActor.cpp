#include "EGlobal.h"
#include "EEntityProxyActor.h"


const CResourceActor* EEntityProxyActor::GetActorInfo()
{
	return NULL;
}

IMotionMgr*	EEntityProxyActor::GetMotionMgr()
{
	return NULL;
}

void EEntityProxyActor::ProcessEvent( EntityEvent &event )
{

}

void EEntityProxyActor::Init(IEntity* pEntity)
{
	m_pEntity = pEntity;
	m_pResource = NULL;
}

void EEntityProxyActor::Update(float deltaTime)
{

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
		std::string name = m_pEntity->GetName() + joint.name;
		IEntity* pJointEntity = pEntityMgr->SpawnEntity( name );

		pJointEntity->SetLocalPos( joint.pos );
		pJointEntity->SetLocalRot( joint.rot );
		
		if( joint.parentIndex == -1 )
			m_pEntity->AttachChild( pJointEntity, true );
		else
			m_pJoints[joint.parentIndex]->AttachChild(pJointEntity, true);

		m_pJoints.push_back(pJointEntity);
	}
}

void EEntityProxyActor::Destroy()
{
	IEntityMgr* pEntityMgr = GLOBAL::EntityMgr();
	
	for(int i = m_pJoints.size() -1 ; i > -1 ; --i )
	{
		pEntityMgr->RemoveEntity( m_pJoints[i]->GetID() );
	}

	m_pJoints.clear();
	m_pResource = NULL;
}
