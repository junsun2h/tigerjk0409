#pragma once


class EEntityProxyRender : public IEntityProxyRender
{
	// only object pool can make&delete this class
	friend CObjectPool<EEntityProxyRender>;
	EEntityProxyRender(){}
	~EEntityProxyRender(){}

public:
	void					Init(IEntity* pEntity) override;
	void					Destroy();

	IEntity*				GetEntity()	override						{ return m_pEntity;}
	void					ProcessEvent( EntityEvent &event ) override;

	bool					Insert(long meshID ) override;
	void					Remove(long slot) override;

	const RENDER_ELEMENT_LIST&	GetRenderElements() override		{ return m_vecRenderElement; }

	bool					IsRenderGeometry(long geometryID );
	void					Render() override;

private:

	IEntity*				m_pEntity;
	RENDER_ELEMENT_LIST		m_vecRenderElement;
	long					m_RenderedFrame;
};