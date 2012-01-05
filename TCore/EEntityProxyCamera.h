#pragma once

#include "IEntityProxy.h"

class EEntityProxyCamera : public IEntityProxyCamera
{
public:
	EEntityProxyCamera(std::string& name, long id );

	long					GetID()	override							{ return m_ID; }
	IEntity*				GetEntity()	override						{ return m_pEntity;}
	void					SetEntity(IEntity* pEntity) override;
	void					ProcessEvent( EntityEvent &event ) override;

	void					SetProjParam(float fovy, int width, int height, float nearPlane, float farPlane) override;
	void					SetViewParam(CVector3& eyePos, CVector3& targetPos, CVector3& upVector) override;

	CMatrix					GetViewTransform() override			{ return m_ViewTM; }
	CMatrix					GetProjectionTransform() override	{ return m_ProjTM; }

	float					GetNearClip() override				{ return m_nearClip; }
	float					GetFarClip() override				{ return m_farClip; }
	float					GetFovy() override					{ return m_Fovy;}
	long					GetLastTransformChangedFrame() override	{ return m_FrameTransformChanged; }

private:
	long					m_ID;
	IEntity*				m_pEntity;
	std::string				m_Name;

private:
	CMatrix					m_ViewTM;
	CMatrix					m_ProjTM;
		
	float					m_nearClip;
	float					m_farClip;
	float					m_Fovy;

	long					m_FrameTransformChanged;
};