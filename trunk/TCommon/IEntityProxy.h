#pragma once

#include <string>
#include "CResource.h"
#include "CCamera.h"


struct IMotionMgr;
struct IEntity;
struct EntityEvent;
enum eENTITY_EVENT_ID;


enum eENTITY_PROXY_TYPE
{
	ENTITY_PROXY_ACTOR,
	ENTITY_PROXY_RENDER,
	ENTITY_PROXY_CAMERA,

	NUM_ENTITY_PROXY_TYPE
};


struct IEntityProxy
{
	virtual ~IEntityProxy(){}

	virtual eENTITY_PROXY_TYPE	GetType() = 0;
	virtual const char*			GetTypeString() = 0;
	virtual IEntity*			GetEntity() = 0;
	virtual void				Init(IEntity* pEntity) =0;

	virtual	void				ProcessEvent( EntityEvent &event ) = 0;
};

struct IEntityProxyActor : IEntityProxy
{
	virtual eENTITY_PROXY_TYPE		GetType()			{ return ENTITY_PROXY_ACTOR; };
	virtual const char*				GetTypeString()		{ return "ENTITY_PROXY_ACTOR"; }

	virtual const CResourceActor*	GetActorInfo() = 0;
	virtual IMotionMgr*				GetMotionMgr() = 0;
	virtual void					Update(float deltaTime) = 0;
};


struct IEntityProxyCamera : IEntityProxy
{
	virtual eENTITY_PROXY_TYPE		GetType() override				{ return ENTITY_PROXY_CAMERA; };
	virtual const char*				GetTypeString()					{ return "ENTITY_PROXY_CAMERA"; }

	virtual void					SetProjParam(float fovy, int width, int height, float nearPlane, float farPlane) = 0;
	virtual void					SetViewParam(CVector3& eyePos, CVector3& targetPos, CVector3& upVector) = 0;

	virtual long					GetLastTransformChangedFrame() =0;
	virtual	const CCAMERA_DESC&		GetDesc() =0;

	virtual void					GetPickRayFromScreen(UINT screenX, UINT screenY, CVector3& origin, CVector3& direction) = 0;
};


struct CRenderElement
{
	CResourceGeometry*	pGeometry;
	CResourceMtrl*		pMtrl;
};

typedef	std::vector<CRenderElement>	RENDER_ELEMENT_LIST;

struct IEntityProxyRender : IEntityProxy
{
	virtual eENTITY_PROXY_TYPE		GetType() override				{ return ENTITY_PROXY_RENDER; };
	virtual const char*				GetTypeString()					{ return "ENTITY_PROXY_RENDER"; }

	virtual const RENDER_ELEMENT_LIST&	GetRenderElements() = 0;
	
	virtual	bool					Insert( long meshID ) = 0;
	virtual void					Remove(long slot) = 0;
	virtual void					Render() = 0;
};