#pragma once

#include "CDefine.h"
#include "IEntityProxy.h"
#include "IAABB.h"
#include "CColiision.h"

enum eENTITY_TYPE
{
	E_EVENT_TRANSFORM_CHANGED,
};


struct EntityEvent
{
	eENTITY_TYPE	type;
	byte*			pData;
	UINT			dataSize;
	UINT			flags;
};

enum eENTITY_UPDATE_FLAG
{
	ENTITY_UPDATE_NONE = 0,
	ENTITY_PRE_UPDATE =	BIT(1),
	ENTITY_PRO_UPDATE =	BIT(2),
	ENTITY_UPDATE_BOTH = BIT(3)
};

typedef std::vector<IEntity*>	TYPE_ENTITY_LIST;

struct IEntity
{
	virtual	UINT			GetID()	= 0;
	virtual std::string		GetName() = 0;
	virtual	bool			IsVisible() = 0;

	//////////////////////////////////////////////////////////////////////////
	// entity properties
	virtual	void			SendEvent( EntityEvent &e ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// proxy functions
	virtual IEntityProxy*	GetProxy( eENTITY_PROXY_TYPE type, bool bCreate = false ) = 0;
	virtual	IEntityProxy*	CreateProxy( eENTITY_PROXY_TYPE type ) = 0;
	virtual	bool			DeleteProxy( eENTITY_PROXY_TYPE type ) = 0;
	virtual void			DeleteAllProxy() = 0;

	//////////////////////////////////////////////////////////////////////////
	// transform functions
	virtual CVector3		GetLocalPos() = 0;
	virtual CQuat			GetLocalRot() = 0;
	virtual CVector3		GetLocalScale() = 0;
	virtual XMMATRIX		GetLocalTM() = 0;

	virtual CVector3		GetWorldPos() = 0;
	virtual CQuat			GetWorldRot() = 0;
	virtual XMMATRIX		GetWorldTM() = 0;

	virtual void			SetLocalPos(const CVector3& _pos) = 0;
	virtual void			SetLocalScale(const CVector3& _scale) = 0;
	virtual void			SetLocalRot(const CQuat& _rot) = 0;
	virtual void			SetLocalTM(const XMMATRIX& tm) = 0;
	virtual void			SetWorldPos(const CVector3& _pos) = 0;
	virtual void			SetWorldRot(const CQuat& _rot) = 0;
	virtual void			SetWorldTM(const XMMATRIX& tm) = 0;

	virtual void			UpdateLocalTM() = 0;
	virtual void			UpdateWorldTM() = 0;

	virtual void			MoveLocalAxis(float x, float y, float z) = 0;
	virtual void			RotateLocalAxis(CVector3 axis, float radian) = 0;

	//////////////////////////////////////////////////////////////////////////
	// hierarchy functions
	virtual void			Reparent( IEntity* _pParent ) = 0;
	virtual void			AttachChild( IEntity* pChild ) = 0;
	virtual void			DetachChild( IEntity* pChild ) = 0;
	virtual void			DetachAllChild() = 0;
	
	virtual IEntity*		GetChild(UINT index ) = 0;
	virtual int				GetChildCount()	 = 0;
	virtual IEntity*		GetParent() = 0;

	//////////////////////////////////////////////////////////////////////////
	// AABB
	virtual const IAABB*	GetWorldAABB() = 0;
	virtual const IAABB*	GetLocalAABB() = 0;
	virtual const IAABB*	GetLocalEntityAABB() = 0;

	virtual bool			Pick(CCollisionDescLine& desc, TYPE_ENTITY_LIST& list) = 0;

};

