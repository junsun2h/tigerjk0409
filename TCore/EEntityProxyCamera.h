#pragma once

#include "IEntityProxy.h"
#include "CUnitPool.h"


class EEntityProxyCamera : public IEntityProxyCamera
{
	// only object pool can make&delete this class
	friend CObjectPool<EEntityProxyCamera>;
	EEntityProxyCamera(){}
	~EEntityProxyCamera(){}

public:
	void					Init(IEntity* pEntity) override;
	void					Destroy(){}

	IEntity*				GetEntity()	override						{ return m_pEntity;}
	void					ProcessEvent( EntityEvent &event ) override;

	void					SetProjParam(float fovy, int width, int height, float nearPlane, float farPlane) override;
	void					SetViewParam(CVector3& eyePos, CVector3& targetPos, CVector3& upVector) override;
	const CCAMERA_DESC&		GetDesc() override		{ return m_Desc; }

	long					GetLastTransformChangedFrame() override	{ return m_FrameTransformChanged; }
	
private:
	void					SetViewDescFromWorldMatrix();

	IEntity*				m_pEntity;
	CCAMERA_DESC			m_Desc;
	long					m_FrameTransformChanged;
};