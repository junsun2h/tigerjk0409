#include "CResource.h"
#include "CRenderElement.h"

#include "IEntity.h"
#include "IEntityProxy.h"
#include "IAssetMgr.h"
#include "IRenderer.h"
#include "IRDevice.h"
#include "IShader.h"

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

bool EEntityProxyRender::CreateRenderElement(long meshID, int meshSlotInActor )
{
	IAssetMgr* pAssetMgr = GLOBAL::AssetMgr();
	IShaderMgr* pShaderMgr = GLOBAL::RDevice()->GetShaderMgr();
	CResourceMesh* pMesh = (CResourceMesh*)pAssetMgr->GetResource(RESOURCE_MESH, meshID);

	for( UINT i=0; i < pMesh->goemetries.size() ; ++i )
	{
		if( IsAlreadyInserted( pMesh->goemetries[i]->RID ) )
			continue;

		CRenderElement item;
		item.pGeometry = pMesh->goemetries[i];
		item.meshSlot = meshSlotInActor;

		memcpy( &item.material, pMesh->goemetries[i]->pMaterial, sizeof(CResourceMtrl) );
		item.InitFlag();

		pShaderMgr->CheckShader( &item );
		m_vecRenderElement.push_back( item );
	}

	m_pEntity->ADDLocalEntityAABB( pMesh->min, pMesh->max );

	return true;
}

void EEntityProxyRender::Remove(long slot)
{
}

bool EEntityProxyRender::IsAlreadyInserted(long geometryID )
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
	long currentFrame = GLOBAL::Engine()->GetCurrentFrame();

	if( m_RenderedFrame == currentFrame )
		return;

	m_RenderedFrame = currentFrame; 
	CCommandBuffer<eRENDER_COMMAND>* pCommandQueue = GLOBAL::Renderer()->GetFillCommandQueue();
	
	XMMATRIX matrixBuf[100];
	
	for( UINT i=0; i < m_vecRenderElement.size(); ++i)
	{
		CRenderElement& renderElement = m_vecRenderElement[i];
		renderElement.worldMatrix =  GetEntity()->GetWorldTM();

		pCommandQueue->AddCommandStart(RC_DRAW_RENDER_ELEMENT);
		pCommandQueue->AddParam( renderElement );

		// additional information
		if( renderElement.pGeometry->IsSkinMesh() )
		{
			IEntityProxyActor* pActor = (IEntityProxyActor*)GetEntity()->GetProxy(ENTITY_PROXY_ACTOR);
			if( pActor == NULL )
			{
				assert(0);
				return;
			}

			SKIN_REF_MATRIX* pSkinRefMatrix = pActor->GetSkinReferenceMatrix( renderElement.meshSlot );
	
			UINT size = pSkinRefMatrix->size();
			for( UINT si = 0; si < size; ++si)
				memcpy( &matrixBuf[si], (*pSkinRefMatrix)[si], sizeof(XMMATRIX) );

			pCommandQueue->AddData( matrixBuf, sizeof(XMMATRIX) * size );
		}

		pCommandQueue->AddCommandEnd();
	}
}
