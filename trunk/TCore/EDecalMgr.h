#pragma once


class EDecalMgr : public IDecalMgr
{
	void			Destroy() override;
	
	CDecalDesc*		Create() override;
	CDecalDesc*		Find(UINT handle) override;
	
	void			Remove(UINT handle) override;

	void			UpdateVisible() override;
};
