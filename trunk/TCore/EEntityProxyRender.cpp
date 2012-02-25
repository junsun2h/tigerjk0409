#include "CResource.h"

#include "IEntity.h"
#include "IEntityProxy.h"
#include "IAssetMgr.h"
#include "IRenderCommand.h"

#include "EGlobal.h"
#include "EEntityProxyRender.h"


void EEntityProxyRender::Init(IEntity* pEntity)
{
	m_RenderedFrame = -1;
	m_pEntity = pEntity;
}

void EEntityProxyRender::ProcessEvent( EntityEvent &event )
{

}

bool EEntityProxyRender::Insert(long meshID )
{
	IAssetMgr* pAssetMgr = GLOBAL::AssetMgr();
	CResourceMesh* pMesh = (CResourceMesh*)pAssetMgr->GetResource(RESOURCE_MESH, meshID);

	for( int i=0; i < pMesh->geometryNum ; ++i )
	{
		CRenderElement item;
		
		item.pGeometry = (CResourceGeometry*)pAssetMgr->GetResource( RESOURCE_GEOMETRY, pMesh->goemetries[i] );

		if( IsRenderGeometry( item.pGeometry->RID ) )
			continue;

		item.pMtrl = (CResourceMtrl*)pAssetMgr->GetResource( RESOURCE_MATERIAL, item.pGeometry->defaultMtrl);

		m_vecRenderElement.push_back( item );
	}

	m_pEntity->ADDLocalEntityAABB( pMesh->min, pMesh->max );

	return true;
}

void EEntityProxyRender::Remove(long slot)
{
}

bool EEntityProxyRender::IsRenderGeometry(long geometryID )
{
	for( UINT i=0; i < m_vecRenderElement.size(); ++i)
	{
		if( m_vecRenderElement[i].pGeometry->RID == geometryID )
			return true;
	}

	return false;
}

void EEntityProxyRender::Render()
{
	CCommandBuffer<eRENDER_COMMAND>* pCommandQueue = GLOBAL::AsyncRenderer()->GetFillCommandQueue();
	long currentFrame = GLOBAL::Engine()->GetCurrentFrame();

	if( m_RenderedFrame == currentFrame )
		return;

	m_RenderedFrame = currentFrame; 

	for( UINT i=0; i < m_vecRenderElement.size(); ++i)
	{
		if( m_vecRenderElement[i].pGeometry->IsSkinedMesh() )
		{
			IEntityProxyActor* pActor = (IEntityProxyActor*)GetEntity()->GetProxy(ENTITY_PROXY_ACTOR);
			if( pActor == NULL )
			{
				assert(0);
				return;
			}
			MOTION_POSE_MATRIX* pMotionPoseMatrix = pActor->GetAnimatoinMatrix();

			CRenderParamSkin param;
			param.pGeometry = m_vecRenderElement[i].pGeometry;
			param.pMaterial = m_vecRenderElement[i].pMtrl;
			param.worldTM = GetEntity()->GetWorldTM();
			param.refSkinTM = pMotionPoseMatrix->GetData();
			param.refSkinTMCount = pMotionPoseMatrix->GetSize();

			pCommandQueue->AddCommandStart(RC_DRAW_OBJECT_SKIN);
			pCommandQueue->AddParam( param);
			pCommandQueue->AddCommandEnd();
		}
		else
		{
			CRenderParam param;
			param.pGeometry = m_vecRenderElement[i].pGeometry;
			param.pMaterial = m_vecRenderElement[i].pMtrl;
			param.worldTM = GetEntity()->GetWorldTM();

			pCommandQueue->AddCommandStart(RC_DRAW_OBJECT);
			pCommandQueue->AddParam( param);
			pCommandQueue->AddCommandEnd();
		}
	}
}
