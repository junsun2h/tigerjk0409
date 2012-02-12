#include <algorithm>

#include "CResource.h"

#include "IEntity.h"
#include "IEntityMgr.h"
#include "IEntityProxy.h"
#include "IMotionInstance.h"

#include "EGlobal.h"
#include "EMotionInstance.h"
#include "EEntityProxyActor.h"


CObjectPool<EMotionInstance>	g_MemPoolMotionInstance(1000);



//----------------------------------------------------------------------------------------------------------------------------
bool SORT_MOTION_INSTANCE(IMotionInstance* pA, IMotionInstance* pB)
{
	if( pA->GetDesc()->Priority == pB->GetDesc()->Priority)
		return pA->GetState()->id < pB->GetState()->id;

	return pA->GetDesc()->Priority < pB->GetDesc()->Priority;
}


//----------------------------------------------------------------------------------------------------------------------------
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

	m_AnimationMatrix.reserve( pResource->jointList.size() );
}

void EEntityProxyActor::Destroy()
{
	IEntityMgr* pEntityMgr = GLOBAL::EntityMgr();
	
	for(int i = m_pJointEntities.size() -1 ; i > -1 ; --i )
	{
		pEntityMgr->RemoveEntity( m_pJointEntities[i]->GetID() );
	}

	m_pJointEntities.clear();
	m_PlayingMotionList.clear();
	m_pEntity = NULL;
	m_pResource = NULL;
	m_AnimationMatrix.clear();
	m_bPause = false;
}

void EEntityProxyActor::Play(CMotionDesc* pDesc)
{
	m_bPause = false;

	static long generateTiming = 0;

	EMotionInstance* pMotionInstance = g_MemPoolMotionInstance.GetNew();
	pMotionInstance->Init(pDesc, generateTiming++);

	m_PlayingMotionList.push_back(pMotionInstance);
	sort( m_PlayingMotionList.begin(), m_PlayingMotionList.end(), SORT_MOTION_INSTANCE);
}

void EEntityProxyActor::Stop()
{

}

bool EEntityProxyActor::IsPlaying()
{
	if( m_PlayingMotionList.size() == 0)
		return false;
	else
		return true;
}

void EEntityProxyActor::Update(float deltaTime)
{
	if( m_bPause == true || IsPlaying() == false)
		return;

	if( m_pEntity->IsCulled() )
		VisibleUpdate(deltaTime);
	else
		CulledUpdate(deltaTime);
}

void EEntityProxyActor::VisibleUpdate(float deltaTime)
{
	UINT jointCount = m_pJointEntities.size();
	
	// reset animation matrix to Figure mode
	for(UINT i=0; i < jointCount; ++i)
	{
		m_AnimationMatrix[i].pos = m_pResource->jointList[i].pos;
		m_AnimationMatrix[i].rot = m_pResource->jointList[i].rot;
	}

	// Update animation
	MOTION_INSTANCE_LIST::iterator itr = m_PlayingMotionList.begin();
	for(; itr != m_PlayingMotionList.end(); )
	{
		IMotionInstance* pMotionInstance = *itr;
		eMOTION_PLAY_STATE motionState = pMotionInstance->VisibleUpdate(deltaTime);

		if( motionState == MOTION_READY )
			continue;

		if( motionState == MOTION_STOPPED || motionState == MOTION_PLAY_INVAILD)
		{
			g_MemPoolMotionInstance.Remove( *itr );
			itr = m_PlayingMotionList.erase(itr);
			continue;
		}

		pMotionInstance->ApplyToMotionPose( &m_AnimationMatrix );
		itr++;
	}

	ApplyAnimationToActor();
}

void EEntityProxyActor::CulledUpdate(float deltaTime)
{
	MOTION_INSTANCE_LIST::iterator itr = m_PlayingMotionList.begin();
	for(; itr != m_PlayingMotionList.end(); )
	{
		IMotionInstance* pMotionInstance = *itr;
		eMOTION_PLAY_STATE motionState = pMotionInstance->CulledUpdate(deltaTime);

		if( motionState == MOTION_READY )
			continue;

		if( motionState == MOTION_STOPPED || motionState == MOTION_PLAY_INVAILD)
		{
			g_MemPoolMotionInstance.Remove( *itr );
			itr = m_PlayingMotionList.erase(itr);
			continue;
		}

		itr++;
	}
}


void EEntityProxyActor::ApplyAnimationToActor()
{
	UINT jointCount = m_pJointEntities.size();

	for(UINT i=0; i < jointCount; ++i)
	{
		XMMATRIX tm = XMMatrixRotationQuaternion( m_AnimationMatrix[i].rot.m128 );
		tm.r[3] = m_AnimationMatrix[i].pos.ToXMVEECTOR();

		m_pJointEntities[i]->SetLocalTM(tm);
	}
}
