#pragma once



class ELightMgr : public ILightMgr
{
	void				Destroy() override;

	CLightDesc*			Create() override;
	CLightDesc*			Find(UINT handle) override;
	
	void				Remove(UINT handle) override;

	void				UpdateVisible() override;
	const LIGHT_LIST*	GetVisibleLights() override				{ return &m_VisibleList;}

	void				GetAffectLight(IEntity* pEntity, LIGHT_LIST* pLightList) override;

	LIGHT_LIST			m_VisibleList;
};