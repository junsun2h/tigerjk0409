#pragma once


#include "IEntityProxy.h"
#include "CUnitPool.h"
#include <atlcoll.h>


struct ERenderItem
{
	long	geometryID;
	long	mtrlID;
};

class EEntityProxyRender : public IEntityProxyRender
{
	// only object pool can make&delete this class
	friend CObjectPool<EEntityProxyRender>;
	EEntityProxyRender(){}
	~EEntityProxyRender(){}

public:
	void			Init(std::string& name, long id );
	void			Destroy();

	long			GetID()	override							{ return m_ID; }
	IEntity*		GetEntity()	override						{ return m_pEntity;}
	void			SetEntity(IEntity* pEntity) override;

	void			ProcessEvent( EntityEvent &event ) override;

	int				GetTotalSlot() override						{ return m_RenderItems.GetCount(); }
	bool			AddGeometry(int slot, long geometryID, long mtrlID ) override;
	void			Remove(long slot) override;


private:
	long				m_ID;
	IEntity*			m_pEntity;
	std::string			m_Name;

private:
	typedef	ATL::CAtlMap<int, ERenderItem>	RENDER_ITEM_MAP;

	RENDER_ITEM_MAP		m_RenderItems;
};