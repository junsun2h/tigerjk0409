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
	const CCAMERA_DESC&		GetDesc() override		{ return m_Desc; }

	long					GetLastTransformChangedFrame() override	{ return m_FrameTransformChanged; }

private:
	void					SetViewDescFromWorldMatrix();

	long					m_ID;
	IEntity*				m_pEntity;
	std::string				m_Name;

private:
	CCAMERA_DESC			m_Desc;
	long					m_FrameTransformChanged;
};