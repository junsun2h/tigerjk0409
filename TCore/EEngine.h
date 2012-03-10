#pragma once


class EEngine : public IEngine
{
public:
	EEngine();

	bool				StartUp(const CENGINE_INIT_PARAM* pParam) override;
	bool				ShutDown() override;
	void				Resize(UINT width, UINT height) override;

	IAssetMgr*			AssetMgr() override;
	IEntityMgr*			EntityMgr() override;
	IRenderer*			Renderer() override;
	ILightMgr*			LightMgr() override;
	CTimer*				GlobalTimer() override;
	ILoader*			Loader() override;
	ISpaceMgr*			SpaceMgr() override;
	long				GetCurrentFrame() override;

	void				UpdateAndRender(CCAMERA_DESC* pCameraDesc, IRenderingCallback* pRenderCallback) override;

private:
	long				m_CurrentFrame;
	CTimer				m_GlobalTimer;
};