#pragma once

#include "CMathType.h"
#include "IEntityProxy.h"



enum ENTITY_EVENT
{
	E_EVENT_TRANSFORM_CHANGED,
};


struct IEntity
{
	virtual	UINT			GetID()	= 0;
	virtual void			Update() = 0; 	// if entity need to be updated per frame, EntityMgr will call this function
	virtual bool			IsUsingPerFrameUpdate() = 0;

	virtual const char*		GetName() = 0;
	
	virtual	void			SendEvent( ENTITY_EVENT &e ) = 0;

	virtual IEntityProxy*	GetProxy( ENTITY_PROXY_TYPE type ) = 0;
	virtual void			SetProxy( ENTITY_PROXY_TYPE type, IEntityProxy *pProxy) = 0;

	//////////////////////////////////////////////////////////////////////////
	// transform functions
	virtual CVector3	GetLocalPos() = 0;
	virtual CQuat		GetLocalRot() = 0;
	virtual CVector3	GetLocalScale() = 0;
	virtual CMatrix		GetLocalTM() = 0;

	virtual CVector3	GetWorldPos() = 0;
	virtual CQuat		GetWorldRot() = 0;
	virtual CMatrix		GetWorldTM() = 0;

	virtual void		SetLocalPos(const CVector3& _pos) = 0;
	virtual void		SetLocalScale(const CVector3& _scale) = 0;
	virtual void		SetLocalRot(const CQuat& _rot) = 0;
	virtual void		SetLocalTM(const CMatrix& tm) = 0;
	virtual void		SetWorldTM(const CMatrix& tm) = 0;

	virtual void		UpdateLocalTM() = 0;
	virtual void		UpdateWorldTM() = 0;

	//////////////////////////////////////////////////////////////////////////
	// hierarchy functions
	virtual void		Reparent( IEntity* _pParent ) = 0;
	virtual void		AttachChild( IEntity* pChild ) = 0;
	virtual void		DetachChild( IEntity* pChild ) = 0;
	virtual void		DetachAllChild() = 0;
	
	virtual IEntity*	GetChild(UINT index ) = 0;
	virtual int			GetChildCount()	 = 0;
	virtual IEntity*	GetParent() = 0;

};