#pragma once

#include <vector>
#include "IEntity.h"
#include "CGrowableArray.h"


class EEntity : public IEntity
{
public:
	EEntity(std::string& name, UINT id);
	virtual ~EEntity();

	UINT			GetID() override			{ return m_ID; };
	std::string		GetName() override			{ return m_Name; }

	void			SendEvent( EntityEvent &e ) override;

private:
	UINT			m_ID;
	std::string		m_Name;

	CGrowableArray<EntityEvent> m_EventQueue;	// event stack for additional events while event is processing

public:
	//////////////////////////////////////////////////////////////////////////
	// proxy functions
	IEntityProxy*	CreateProxy( ENTITY_PROXY_TYPE	type, std::string& strResource) override;
	IEntityProxy*	GetProxy( ENTITY_PROXY_TYPE type ) override { return m_Proxyes[type]; }
	void			SetProxy( ENTITY_PROXY_TYPE type, IEntityProxy *pProxy) override;

private:
	IEntityProxy*	m_Proxyes[NUM_ENTITY_PROXY_TYPE];

public:
	//////////////////////////////////////////////////////////////////////////
	// transform functions
	CVector3		GetLocalPos() override		{ return m_LocalPos;	}
	CQuat			GetLocalRot() override		{ return m_LocalRotation;	}
	CVector3		GetLocalScale() override	{ return m_LocalScale;	}
	CMatrix			GetLocalTM() override		{ return m_LocalTM;	}

	CVector3		GetWorldPos() override		{ return m_WorldPos;	}
	CQuat			GetWorldRot() override		{ return m_WorldRotation;	}
	CMatrix			GetWorldTM() override		{ return m_WorldTM;	}

	void			SetLocalPos(const CVector3& _pos) override;
	void			SetLocalScale(const CVector3& _scale) override;
	void			SetLocalRot(const CQuat& _rot) override;
	void			SetLocalTM(const CMatrix& tm) override;
	void			SetWorldTM(const CMatrix& tm) override;

	void			UpdateLocalTM() override;
	void			UpdateWorldTM() override;

private:
	CVector3	m_LocalPos;
	CVector3	m_LocalScale;
	CQuat		m_LocalRotation;
	CMatrix		m_LocalTM;

	CVector3	m_WorldPos;
	CVector3	m_WorldScale;
	CQuat		m_WorldRotation;
	CMatrix		m_WorldTM;

	UINT		m_SpaceID;		// Octree space id

public:
	//////////////////////////////////////////////////////////////////////////
	// hierarchy functions
	void			Reparent( IEntity* _pParent ) override;
	void			AttachChild( IEntity* pChild ) override;
	void			DetachChild( IEntity* pChild ) override;
	void			DetachAllChild()  override;

	IEntity*		GetChild(UINT index )  override;
	int				GetChildCount() override	{ return m_Children.size(); }
	IEntity*		GetParent()	override		{ return m_pParent; }

private:
	IEntity*				m_pParent;
	std::vector<IEntity*>	m_Children;
};

