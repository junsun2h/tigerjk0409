#pragma once


class EActorMgr : public IActorMgr
{
public:
	IEntityProxyActor*	GetNew();
	void				Remove(IEntityProxy* pActor);

	void				Update(float timeDelta);
};