#pragma once

#include "CGrowableArray.h"


struct IEntity;
struct EntityEvent;
struct CCAMERA_DESC;
enum eENTITY_EVENT_ID;
class CResourceActor;
class CResourceGeometry;
class CResourceMtrl;

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

struct CMotionDesc;
struct IMotionInstance;
typedef std::vector<IMotionInstance*>	MOTION_INSTANCE_LIST;
typedef std::vector<IEntity*>			JOINT_ENTITY_LIST;
typedef CGrowableArray<XMMATRIX>		MOTION_POSE_MATRIX;


struct IEntityProxyActor : IEntityProxy
{
	virtual eENTITY_PROXY_TYPE		GetType()			{ return ENTITY_PROXY_ACTOR; };
	virtual const char*				GetTypeString()		{ return "ENTITY_PROXY_ACTOR"; }

	virtual	JOINT_ENTITY_LIST*		GetJointEntities() =0;
	virtual MOTION_POSE_MATRIX*		GetAnimatoinMatrix() =0;

	virtual const CResourceActor*	GetResource() = 0;
	virtual	void					SetActor(const CResourceActor* pResource) = 0;

	virtual void					Play(CMotionDesc* pdesc) = 0;
	virtual void					Freeze() = 0;
	virtual void					Stop() = 0;
	virtual void					Resume() =0;
	virtual bool					IsPlaying() = 0;

	virtual MOTION_INSTANCE_LIST*	GetPlayingMotions() = 0;

	virtual	void					Update(float deltaTime) = 0;
};


struct IEntityProxyCamera : IEntityProxy
{
	virtual eENTITY_PROXY_TYPE		GetType() override				{ return ENTITY_PROXY_CAMERA; };
	virtual const char*				GetTypeString()					{ return "ENTITY_PROXY_CAMERA"; }

	virtual void					SetProjParam(float fovy, int width, int height, float nearPlane, float farPlane) = 0;
	virtual void					SetViewParam(CVector3& eyePos, CVector3& targetPos, CVector3& upVector) = 0;

	virtual long					GetLastTransformChangedFrame() =0;
	virtual	const CCAMERA_DESC*		GetDesc() =0;
	virtual	void					CopyDesc(CCAMERA_DESC* pDesc) = 0;
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