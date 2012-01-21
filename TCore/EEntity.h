#pragma once

#include <vector>
#include <atlcoll.h>
#include "IEntity.h"
#include "CGrowableArray.h"
#include "CUnitPool.h"
#include "EAABB.h"


typedef std::list<UINT>	TYPE_SPACE_IDS;


class EEntity : public IEntity
{
	// only object pool can make&delete this class
	friend CObjectPool<EEntity>;
	EEntity();
	virtual ~EEntity();

public:
	void			Init(std::string& name, UINT id);

	UINT			GetID() override			{ return m_ID; };
	std::string		GetName() override			{ return m_Name; }

	TYPE_SPACE_IDS*	GetSpaceIDList()			{ return &m_SpaceIDList;}
	void			AddSpaceID(UINT id);
	void			RemoveSpaceID(UINT id);

	void			Destroy();
	void			SendEvent( EntityEvent &e ) override;
	bool			IsVisible() override;

private:
	UINT			m_ID;
	std::string		m_Name;
	std::list<UINT>	m_SpaceIDList;

	CGrowableArray<EntityEvent> m_EventQueue;	// event stack for additional events while event is processing

public:
	//////////////////////////////////////////////////////////////////////////
	// proxy functions
	IEntityProxy*	GetProxy( eENTITY_PROXY_TYPE type ) override;
	void			SetProxy( IEntityProxy *pProxy) override;
	void			DeleteAllPrxoy() override;

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
	CQuat			GetWorldRot() override		{ return m_WorldRotation;	}
	XMMATRIX		GetWorldTM() override		{ return m_WorldTM;	}

	void			SetLocalPos(const CVector3& _pos) override;
	void			SetLocalScale(const CVector3& _scale) override;
	void			SetLocalRot(const CQuat& _rot) override;
	void			SetLocalTM(const XMMATRIX& tm) override;
	void			SetWorldPos(const CVector3& _pos) override;
	void			SetWorldRot(const CQuat& _rot) override;
	void			SetWorldTM(const XMMATRIX& tm) override;

	void			MoveLocalAxis(float x, float y, float z) override;
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
	CQuat			m_WorldRotation;
	XMMATRIX		m_WorldTM;
	
public:
	//////////////////////////////////////////////////////////////////////////
	// hierarchy
	void			Reparent( IEntity* _pParent ) override;
	void			AttachChild( IEntity* pChild ) override;
	void			DetachChild( IEntity* pChild ) override;
	void			DetachAllChild()  override;

	IEntity*		GetChild(UINT index )  override;
	int				GetChildCount() override	{ return m_Children.size(); }
	IEntity*		GetParent()	override		{ return m_pParent; }

private:
	IEntity*				m_pParent;

	typedef std::vector<EEntity*>	TYPE_ENTITY_LIST;
	TYPE_ENTITY_LIST		m_Children;

public:
	//////////////////////////////////////////////////////////////////////////
	// AABB
	const IAABB*	GetWorldAABB() override		{ return &m_WorldAABB; }
	const IAABB*	GetLocalAABB() override		{ return &m_LocalAABB; }

private:
	EAABB			m_WorldAABB;
	EAABB			m_LocalAABB;
};

