#include "CResource.h"

#include "IEntity.h"
#include "IEntityProxy.h"
#include "IAssetMgr.h"
#include "IRenderer.h"

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

bool EEntityProxyRender::CreateRenderElement(long meshID, int indexInActor )
{
	IAssetMgr* pAssetMgr = GLOBAL::AssetMgr();
	CResourceMesh* pMesh = (CResourceMesh*)pAssetMgr->GetResource(RESOURCE_MESH, meshID);

	for( UINT i=0; i < pMesh->goemetries.size() ; ++i )
	{
		CRenderElement item;
		
		item.pGeometry = pMesh->goemetries[i];

		if( IsRenderGeometry( item.pGeometry->RID ) )
			continue;

		item.pMtrl = (CResourceMtrl*)pAssetMgr->GetResource( RESOURCE_MATERIAL, item.pGeometry->defaultMtrl);

		if( indexInActor != -1)
			item.IndexInActor = indexInActor;

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
	CCommandBuffer<eRENDER_COMMAND>* pCommandQueue = GLOBAL::Renderer()->GetFillCommandQueue();
	long currentFrame = GLOBAL::Engine()->GetCurrentFrame();

	if( m_RenderedFrame == currentFrame )
		return;

	m_RenderedFrame = currentFrame; 
	XMMATRIX matrixBuf[100]; 

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

			SKIN_REF_MATRIX* pSkinRefMatrix = pActor->GetSkinReferenceMatrix( m_vecRenderElement[i].IndexInActor );
	
			UINT size = pSkinRefMatrix->size();
			for( UINT si = 0; si < size; ++si)
				memcpy( &matrixBuf[si], (*pSkinRefMatrix)[si], sizeof(XMMATRIX) );

			CRenderParamSkin param;
			param.pGeometry = m_vecRenderElement[i].pGeometry;
			param.pMaterial = m_vecRenderElement[i].pMtrl;
			param.worldTM = GetEntity()->GetWorldTM();
			param.refSkinTMCount = size;

			pCommandQueue->AddCommandStart(RC_DRAW_OBJECT_SKIN);
			pCommandQueue->AddParam( param);
			pCommandQueue->AddData(matrixBuf, sizeof(XMMATRIX) * size );
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
