#include "EEntityProxyRender.h"

void EEntityProxyRender::Init(std::string& name, long id )
{
	m_Name = name;
	m_ID = id;
	m_pEntity =NULL;
}

void EEntityProxyRender::Destroy()
{
	m_RenderItems.RemoveAll();
}

void EEntityProxyRender::SetEntity(IEntity* pEntity)
{
	m_pEntity = pEntity;
}

void EEntityProxyRender::ProcessEvent( EntityEvent &event )
{

}

bool EEntityProxyRender::AddGeometry(int slot, long geometryID, long mtrlID )
{
	if( m_RenderItems.Lookup(slot)  == NULL )
	{
		assert(0);
		return false;
	}

	ERenderItem newItem;
	newItem.geometryID = geometryID;
	newItem.mtrlID = mtrlID;

	m_RenderItems.SetAt(slot, newItem);

	return true;
}

void EEntityProxyRender::Remove(long slot)
{
	if( m_RenderItems.Lookup(slot)  == NULL )
		return;

	m_RenderItems.RemoveKey(slot);
}