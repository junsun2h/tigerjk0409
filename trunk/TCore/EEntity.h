#pragma once


class EEntity : public IEntity
{
	// only object pool can make&delete this class
	friend CObjectPool<EEntity>;
	EEntity();
	virtual ~EEntity();

public:
	void			Init(std::string& name, UINT id) override;

	UINT			GetID() override			{ return m_ID; };
	std::string		GetName() override			{ return m_Name; }

	void			SendEvent( EntityEvent &e ) override;
	bool			IsVisible() override;

private:
	UINT			m_ID;
	std::string		m_Name;
	std::list<UINT>	m_SpaceIDList;

	CGrowableArray<EntityEvent> m_EventQueue;	// event stack for additional events while event is processing

public:
	//////////////////////////////////////////////////////////////////////////
	//  culling information

	TYPE_SPACE_IDS*	GetSpaceIDList() override		{ return &m_SpaceIDList;}
	void			AddSpaceID(UINT id) override;
	void			RemoveSpaceID(UINT id) override;
	bool			IsCulled() override				{ return m_bCulled; }
	void			SetCull(bool bCulled) override	{ m_bCulled = bCulled;}

private:
	bool			m_bCulled;

public:
	//////////////////////////////////////////////////////////////////////////
	// proxy functions
	IEntityProxy*	GetProxy( eENTITY_PROXY_TYPE type, bool bCreate = false ) override;
	IEntityProxy*	CreateProxy( eENTITY_PROXY_TYPE type ) override;
	bool			DeleteProxy( eENTITY_PROXY_TYPE type ) override;
	void			DeleteAllProxy() override;

private:
	typedef ATL::CAtlMap<eENTITY_PROXY_TYPE, IEntityProxy*>	ENEITY_PROXY_MAP;
	ENEITY_PROXY_MAP	m_ProxyMap;

public:
	//////////////////////////////////////////////////////////////////////////
	// transform
	CVector3		GetLocalPos() override		{ return m_LocalPos;	}
	CQuat			GetLocalRot() override		{ return m_LocalRotation;	}
	CVector3		GetLocalScale() override	{ return m_LocalScale;	}
	XMMATRIX		GetLocalTM() override		{ return m_LocalTM;	}

	CVector3		GetWorldPos() override		{ return m_WorldPos;	}
	CVector3		GetWorldScale() override	{ return m_WorldScale;	}
	CQuat			GetWorldRot() override		{ return m_WorldRotation;	}
	XMMATRIX		GetWorldTM() override		{ return m_WorldTM;	}

	void			SetLocalPos(const CVector3& _pos) override;
	void			SetLocalScale(const CVector3& _scale) override;
	void			SetLocalRot(const CQuat& _rot) override;
	void			SetLocalTM(const XMMATRIX& tm) override;
	void			SetWorldPos(const CVector3& _pos) override;
	void			SetWorldRot(const CQuat& _rot) override;
	void			SetWorldTM(const XMMATRIX& tm) override;

	void			MoveOnLocalAxis(float x, float y, float z) override;
	void			RotateLocalAxis(CVector3 axis, float radian) override;

	void			UpdateLocalTM() override;
	void			UpdateWorldTM() override;

private:
	void			UpdateLocalFromWorld();
	void			OnTransformChanged();

	CVector3		m_LocalPos;
	CVector3		m_LocalScale;
	CQuat			m_LocalRotation;
	XMMATRIX		m_LocalTM;

	CVector3		m_WorldPos;
	CVector3		m_WorldScale;
	CQuat			m_WorldRotation;
	XMMATRIX		m_WorldTM;
	
public:
	//////////////////////////////////////////////////////////////////////////
	// hierarchy
	void			Reparent( IEntity* _pParent, bool keepLocalTM  ) override;
	void			AttachChild( IEntity* pChild , bool keepLocalTM  ) override;
	void			DetachChild( IEntity* pChild ) override;
	void			DetachAllChild()  override;

	IEntity*		GetChild(UINT index )  override;
	int				GetChildCount() override	{ return m_Children.size(); }
	IEntity*		GetParent()	override		{ return m_pParent; }

private:
	EEntity*			m_pParent;
	TYPE_ENTITY_LIST	m_Children;

public:
	//////////////////////////////////////////////////////////////////////////
	// AABB
	const CAABB*	GetWorldAABB() override			{ return &m_WorldAABB; }
	const CAABB*	GetLocalEntityAABB() override	{ return &m_LocalEntityAABB; }
	void			ADDLocalEntityAABB(CVector3 min, CVector3 max) override;
	bool			Pick(CVector3& from, CVector3& to, TYPE_ENTITY_LIST& list) override;

private:
	void			UpdateWorldAABB();

	CAABB			m_WorldAABB;		// including child's Bounding box in world coordinates system
	CAABB			m_LocalAABB;		// including child's Bounding box in local coordinates system
	CAABB			m_LocalEntityAABB;	// this entity's Bounding box in local coordinates system
};

