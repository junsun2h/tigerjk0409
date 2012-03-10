#pragma once



class ELightMgr : public ILightMgr
{
	void			Destroy() override;
	CLightDesc*		Create() override;
	CLightDesc*		Find(UINT handle) override;
	void			Remove(UINT handle) override;
	void			GetAffectLight(IEntity* pEntity, LIGHT_LIST* pLightList) override;
};