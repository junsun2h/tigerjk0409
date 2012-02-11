#include "CResource.h"

#include "IEntity.h"
#include "IRDevice.h"
#include "IEntityProxy.h"
#include "IAssetMgr.h"

#include "EGlobal.h"
#include "EEntityProxyRender.h"



void EEntityProxyRender::Destroy()
{
	m_vecRenderElement.clear();
	memset( m_RenderedFrame, 0, sizeof(m_RenderedFrame) );
}

void EEntityProxyRender::Init(IEntity* pEntity)
{
	memset( m_RenderedFrame, 0, sizeof(m_RenderedFrame) );
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
	IRDevice* pRDevice = GLOBAL::Engine()->RDevice();
	long currentFrame = GLOBAL::Engine()->GetCurrentFrame();

	// don't render if it's already rendered in this frame
	eRENDER_PASS currentPass = pRDevice->GetCurrentPass();
	if( m_RenderedFrame[currentPass] == currentFrame )
		return;

	m_RenderedFrame[currentPass] = currentFrame; 

	for( UINT i=0; i < m_vecRenderElement.size(); ++i)
	{
		pRDevice->RenderElement( m_vecRenderElement[i].pGeometry, 
								m_vecRenderElement[i].pMtrl,
								this);
	}
}
