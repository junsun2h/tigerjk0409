#pragma once

struct IEntityProxyActor;

struct IActorMgr
{
	virtual IEntityProxyActor*	GetNew() =0 ;
	virtual void				Remove(IEntityProxy* pActor)=0;
	virtual void				Update(float timeDelta)=0;
};