#include <algorithm>
#include "CResource.h"

#include "IEntity.h"
#include "IEntityMgr.h"
#include "IEntityProxy.h"
#include "IMotionInstance.h"
#include "IAssetMgr.h"

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
EEntityProxyActor::~EEntityProxyActor()
{
	Destroy();
}

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
	UINT jointCount = pResource->jointList.size();

	m_AnimationMatrix.SetSize( jointCount );
	m_InvBindMatrix.SetSize( jointCount );
	m_AnimationPos.reserve( pResource->jointList.size() );

	//---------------------------------------------------------------------
	// create Joints
	for( UINT i=0 ; i < pResource->jointList.size(); ++i )
	{
		const CJoint& joint = pResource->jointList[i];
		std::string name = m_pEntity->GetName() + "_" + joint.name;
		IEntity* pJointEntity = pEntityMgr->SpawnEntity( name );

		pJointEntity->SetSpaceFlag( ENTITY_NO_SPACE_UPDATE );
		pJointEntity->SetLocalPos( joint.pos );
		pJointEntity->SetLocalRot( joint.rot );
		
		if( joint.parentIndex == -1 )
			m_pEntity->AttachChild( pJointEntity, true );
		else
			m_pJointEntities[joint.parentIndex]->AttachChild(pJointEntity, true);
		
		m_InvBindMatrix.Add( XMMATRIX_UTIL::Inverse(NULL, pJointEntity->GetWorldTM() ) );

		m_pJointEntities.push_back(pJointEntity);
		m_AnimationPos.push_back( CMotionTransform() );
		m_AnimationMatrix.Add( XMMatrixIdentity() );
	}

	//---------------------------------------------------------------------
	// make instance of meshes
	IAssetMgr* pAssetMgr = GLOBAL::AssetMgr();

	IEntityProxyRender* pProxy = (IEntityProxyRender*)m_pEntity->GetProxy(ENTITY_PROXY_RENDER, true);
	for( UINT i=0; i < pResource->meshList.size(); ++i)
	{
		// map bone's map between Actor & Mesh
		SKIN_REF_MATRIX mapBetweenMeshAndActor;
		CResourceMesh* pMesh = (CResourceMesh*)pAssetMgr->GetResource( RESOURCE_MESH, pResource->meshList[i]->RID );
		for( UINT bi = 0; bi < pMesh->skinBoneList.size(); ++bi )
		{
			int index = -1;
			for(UINT ji =0; ji < jointCount; ++ji)
			{
				if( pMesh->skinBoneList[bi] == pResource->jointList[ji].name )
					index = ji;
			}

			if( index == -1 )
			{
				assert(0);
				mapBetweenMeshAndActor.push_back( NULL );
			}
			else
			{
				mapBetweenMeshAndActor.push_back( &m_AnimationMatrix[index] );
			}
		}

		m_SkinMatrices.push_back( mapBetweenMeshAndActor );
		pProxy->CreateRenderElement( pMesh->RID, i );
	}
}

void EEntityProxyActor::Destroy()
{
	m_pJointEntities.clear();

	for( UINT i=0 ; i < m_PlayingMotionList.size(); ++i )
		g_MemPoolMotionInstance.Remove( m_PlayingMotionList[i]);
	m_PlayingMotionList.clear();

	m_AnimationMatrix.Reset();

	m_pEntity = NULL;
	m_pResource = NULL;
	m_AnimationPos.clear();
	m_bPause = false;
}

void EEntityProxyActor::Play(CMotionDesc* pDesc)
{
	m_bPause = false;
	pDesc->pResourceActor = m_pResource;

	static long generateTiming = 0;

	EMotionInstance* pMotionInstance = g_MemPoolMotionInstance.GetNew();
	pMotionInstance->Init(pDesc, generateTiming++);

	m_PlayingMotionList.push_back(pMotionInstance);
	sort( m_PlayingMotionList.begin(), m_PlayingMotionList.end(), SORT_MOTION_INSTANCE);
}

void EEntityProxyActor::Stop()
{
	m_bPause = true;
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
		InvisibleUpdate(deltaTime);
	else
		VisibleUpdate(deltaTime); 
}

void EEntityProxyActor::VisibleUpdate(float deltaTime)
{
	UINT jointCount = m_pJointEntities.size();
	
	// reset animation matrix to Figure mode
	for(UINT i=0; i < jointCount; ++i)
	{
		m_AnimationPos[i].pos = m_pResource->jointList[i].pos;
		m_AnimationPos[i].rot = m_pResource->jointList[i].rot;
	}

	// Update animation
	MOTION_INSTANCE_LIST::iterator itr = m_PlayingMotionList.begin();
	for(; itr != m_PlayingMotionList.end(); )
	{
		IMotionInstance* pMotionInstance = *itr;
		eMOTION_PLAY_STATE motionState = pMotionInstance->Update(deltaTime, true);

		if( motionState == MOTION_READY )
			continue;

		if( motionState == MOTION_STOPPED || motionState == MOTION_PLAY_INVAILD)
		{
			g_MemPoolMotionInstance.Remove( *itr );
			itr = m_PlayingMotionList.erase(itr);
			continue;
		}

		pMotionInstance->ApplyToMotionPose( &m_AnimationPos );
		itr++;
	}

	ApplyAnimationToActor();
}

void EEntityProxyActor::InvisibleUpdate(float deltaTime)
{
	MOTION_INSTANCE_LIST::iterator itr = m_PlayingMotionList.begin();
	for(; itr != m_PlayingMotionList.end(); )
	{
		IMotionInstance* pMotionInstance = *itr;
		eMOTION_PLAY_STATE motionState = pMotionInstance->Update(deltaTime, false);

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
	/*
	for i to weightCount
		v += [v * InvBindWorldi * JointWorldi * invWorldE] * weight

	InvBindWorldi: Inverse world bind-pose matrix of joint i
	JointWorldi: World Transformation matrix of joint i
	invWorldE: Inverse Transformation matrix of Entity
	*/

	UINT jointCount = m_pJointEntities.size();

	XMMATRIX invWorld = XMMATRIX_UTIL::Inverse(NULL, m_pEntity->GetWorldTM() );

	for(UINT i=0; i < jointCount; ++i)
	{
		m_AnimationMatrix[i] = XMMatrixRotationQuaternion( m_AnimationPos[i].rot.m128 );
		m_AnimationMatrix[i].r[3] = m_AnimationPos[i].pos.ToXMVEECTOR();

		m_pJointEntities[i]->SetLocalTM(m_AnimationMatrix[i], true);
	}

	m_pEntity->UpdateWorldTM();

	for(UINT i=0; i < jointCount; ++i)
	{
		m_AnimationMatrix[i] = XMMatrixMultiply( m_InvBindMatrix[i], m_pJointEntities[i]->GetWorldTM() );
		m_AnimationMatrix[i] = XMMatrixMultiply( m_AnimationMatrix[i], invWorld );
	}
}
