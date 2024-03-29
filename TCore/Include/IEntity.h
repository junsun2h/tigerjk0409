#pragma once


enum eENTITY_PROXY_TYPE;
struct CAABB;
struct IEntityProxy;
struct IEntity;

typedef std::list<UINT>	TYPE_SPACE_IDS;


enum eENTITY_EVENT_ID
{
	E_EVENT_TRANSFORM_CHANGED,
};


struct EntityEvent
{
	eENTITY_EVENT_ID	type;
	byte*			pData;
	UINT			dataSize;
	UINT			flags;
};

enum eENTITY_SPACE_FLAG
{
	ENTITY_NO_SPACE_UPDATE =	BIT(1)
};

typedef std::vector<IEntity*>	TYPE_ENTITY_LIST;

struct IEntity
{
	virtual ~IEntity(){}
	virtual	long			GetID()	= 0;
	virtual std::string		GetName() = 0;
	virtual	bool			IsVisible() = 0;

	virtual	void			Init(std::string& name, long id) = 0;

	//////////////////////////////////////////////////////////////////////////
	// culling information
	virtual	TYPE_SPACE_IDS*	GetSpaceIDList() = 0;
	virtual	void			AddSpaceID(UINT id) = 0;
	virtual	void			RemoveSpaceID(UINT id) = 0;
	virtual void			SetSpaceFlag(DWORD flag) = 0;
	virtual DWORD			GetSpaceFlag() = 0;
	virtual bool			IsCulled() =0;
	virtual void			SetCull(bool bCulled) =0;

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
	virtual CVector3		GetWorldScale() = 0;
	virtual XMMATRIX		GetWorldTM() = 0;

	virtual void			SetLocalPos(const CVector3& _pos) = 0;
	virtual void			SetLocalScale(const CVector3& _scale) = 0;
	virtual void			SetLocalRot(const CQuat& _rot) = 0;
	virtual void			SetLocalTM(const XMMATRIX& tm, bool bSkipUpdateWorld = false) = 0;
	virtual void			SetWorldPos(const CVector3& _pos) = 0;
	virtual void			SetWorldRot(const CQuat& _rot) = 0;
	virtual void			SetWorldTM(const XMMATRIX& tm) = 0;

	virtual void			UpdateWorldTM() = 0;

	virtual void			MoveOnLocalAxis(float x, float y, float z) = 0;
	virtual void			RotateLocalAxis(CVector3 axis, float radian) = 0;

	//////////////////////////////////////////////////////////////////////////
	// hierarchy functions
	virtual void			Reparent( IEntity* _pParent, bool keepLocalTM ) = 0;
	virtual void			AttachChild( IEntity* pChild, bool keepLocalTM ) = 0;
	virtual void			DetachChild( IEntity* pChild ) = 0;
	virtual void			DetachAllChild() = 0;
	
	virtual IEntity*		GetChild(UINT index ) = 0;
	virtual int				GetChildCount()	 = 0;
	virtual IEntity*		GetParent() = 0;

	//////////////////////////////////////////////////////////////////////////
	// AABB
	virtual const CAABB*	GetWorldAABB() = 0;
	virtual const CAABB*	GetLocalEntityAABB() = 0;
	virtual void			ADDLocalEntityAABB(CVector3 min, CVector3 max) = 0;

	virtual bool			Pick(CVector3& from, CVector3& to, TYPE_ENTITY_LIST& list) = 0;

};

