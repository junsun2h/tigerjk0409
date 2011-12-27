#pragma once

#include "CDefine.h"
#include "CMathType.h"
#include "IEntityProxy.h"



enum ENTITY_TYPE
{
	E_EVENT_TRANSFORM_CHANGED,
};


struct EntityEvent
{
	ENTITY_TYPE		type;
	byte*			pData;
	UINT			dataSize;
	UINT			flags;
};

enum ENTITY_UPDATE_FLAG
{
	ENTITY_UPDATE_NONE = 0,
	ENTITY_PRE_UPDATE = BIT(1),
	ENTITY_PRO_UPDATE = BIT(2),
	ENTITY_UPDATE_BOTH = BIT(3)
};

struct IEntity
{
	virtual	UINT			GetID()	= 0;
	virtual std::string		GetName() = 0;

	//////////////////////////////////////////////////////////////////////////
	// entity properties
	virtual	void			SendEvent( EntityEvent &e ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// proxy functions
	virtual IEntityProxy*	CreateProxy( ENTITY_PROXY_TYPE	type, std::string& strResource) =0;
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