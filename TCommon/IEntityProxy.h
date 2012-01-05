#pragma once

#include <string>
#include "IResource.h"
#include "IMotionMgr.h"

struct IEntity;
struct EntityEvent;
enum ENTITY_TYPE;


enum ENTITY_PROXY_TYPE
{
	ENTITY_STATIC_MESH,
	ENTITY_ACTOR,
	ENTITY_RENDER,
	ENTITY_CAMERA,
	ENTITY_EFFECT,

	NUM_ENTITY_PROXY_TYPE
};


struct IEntityProxy
{
	virtual ~IEntityProxy(){}

	virtual ENTITY_PROXY_TYPE	GetType() = 0;
	virtual IEntity*			GetEntity() = 0;
	virtual void				SetEntity(IEntity* pEntity) = 0;
	virtual long				GetID()	= 0;

	virtual	void				ProcessEvent( EntityEvent &event ) = 0;
};

struct IEntityProxyActor : IEntityProxy
{
	virtual ENTITY_PROXY_TYPE		GetType()			{ return ENTITY_ACTOR; };

	virtual void					Init(std::string strResource) = 0;
	virtual const CResourceActor*	GetActorInfo() = 0;
	virtual IMotionMgr*				GetMotionMgr() = 0;
	virtual void					Update(float deltaTime) = 0;
};

struct IEntityProxyCamera : IEntityProxy
{
	virtual ENTITY_PROXY_TYPE		GetType() override				{ return ENTITY_CAMERA; };

	virtual void					SetProjParam(float fovy, int width, int height, float nearPlane, float farPlane) = 0;
	virtual void					SetViewParam(CVector3& eyePos, CVector3& targetPos, CVector3& upVector) = 0;

	virtual CMatrix					GetViewTransform() = 0;
	virtual CMatrix					GetProjectionTransform() = 0;

	virtual float					GetNearClip() = 0;
	virtual float					GetFarClip() = 0;
	virtual float					GetFovy() = 0;

	virtual long					GetLastTransformChangedFrame() =0;
};

struct IEntityProxyRender : IEntityProxy
{
	virtual ENTITY_PROXY_TYPE		GetType() override				{ return ENTITY_RENDER; };

	virtual int						GetTotalSlot() = 0;
	virtual	bool					AddGeometry(int slot, long geometryID, long mtrlID ) = 0;
	virtual void					Remove(long slot) = 0;
};