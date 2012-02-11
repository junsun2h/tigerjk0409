#pragma once


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
	const CCAMERA_DESC*		GetDesc() override							{ return &m_Desc; }
	void 					CopyDesc(CCAMERA_DESC* pDesc) override		{ *pDesc = m_Desc; }

	long					GetLastTransformChangedFrame() override		{ return m_FrustrumChanged; }
	void					GetPickRayFromScreen(UINT screenX, UINT screenY, CVector3& origin, CVector3& direction) override;

private:
	void					SetViewDescFromWorldMatrix();

	IEntity*				m_pEntity;
	CCAMERA_DESC			m_Desc;
	long					m_FrustrumChanged;
};