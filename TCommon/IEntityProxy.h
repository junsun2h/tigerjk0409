#pragma once

#include <string>
#include "IResource.h"
#include "IMotionMgr.h"

struct IEntity;
struct EntityEvent;
enum ENTITY_TYPE;


enum ENTITY_PROXY_TYPE
{
	ENTITY_PROXY_ACTOR,
	ENTITY_PROXY_RENDER,
	ENTITY_PROXY_CAMERA,

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
	virtual ENTITY_PROXY_TYPE		GetType()			{ return ENTITY_PROXY_ACTOR; };

	virtual void					Init(std::string strResource) = 0;
	virtual const CResourceActor*	GetActorInfo() = 0;
	virtual IMotionMgr*				GetMotionMgr() = 0;
	virtual void					Update(float deltaTime) = 0;
};

struct CCAMERA_DESC
{
	XMMATRIX ViewTM;
	XMMATRIX ProjTM;
			 
	float	 nearClip;
	float	 farClip;
	float	 Fovy;
};

struct IEntityProxyCamera : IEntityProxy
{
	virtual ENTITY_PROXY_TYPE		GetType() override				{ return ENTITY_PROXY_CAMERA; };

	virtual void					SetProjParam(float fovy, int width, int height, float nearPlane, float farPlane) = 0;
	virtual void					SetViewParam(CVector3& eyePos, CVector3& targetPos, CVector3& upVector) = 0;

	virtual long					GetLastTransformChangedFrame() =0;
	virtual	const CCAMERA_DESC&		GetDesc() =0;
};

struct IEntityProxyRender : IEntityProxy
{
	virtual ENTITY_PROXY_TYPE		GetType() override				{ return ENTITY_PROXY_RENDER; };

	virtual int						GetTotalSlot() = 0;
	virtual	bool					AddGeometry(int slot, long geometryID, long mtrlID ) = 0;
	virtual void					Remove(long slot) = 0;
};