#pragma once

#include <vector>
#include "IEntity.h"


class EEntity : public IEntity
{
public:
	EEntity();
	virtual ~EEntity();

	UINT			GetID() override			{ return m_ID; };
	const char*		GetName() override			{ return m_Name; }

	void			Update() override;
	bool			IsUsingPerFrameUpdate() override;

	void			SendEvent( ENTITY_EVENT &e ) override;

	IEntityProxy*	GetProxy( ENTITY_PROXY_TYPE type ) override { return m_Proxyes[type]; }
	void			SetProxy( ENTITY_PROXY_TYPE type, IEntityProxy *pProxy) override;

private:
	UINT			m_ID;
	const char*		m_Name;

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

