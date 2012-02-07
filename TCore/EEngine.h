#pragma once


class EEngine : public IEngine
{
public:
	EEngine();

	bool				StartUp(const CENGINE_INIT_PARAM &param) override;
	bool				ShutDown() override;
	void				Resize(UINT width, UINT height) override;

	IRDevice*			RDevice() override;
	
	IAssetMgr*			AssetMgr() override;
	IEntityMgr*			EntityMgr() override;

	IRenderHelper*		RenderHelper() override;
	CTimer*				GlobalTimer() override;
	ILoader*			Loader() override;
	IEngineMemoryMgr*	EngineMemoryMgr() override;
	ISpaceMgr*			SpaceMgr() override;
	long				GetCurrentFrame() override;

	void				UpdateAndRender(IEntityProxyCamera* pCamera, IRenderingCallback* pRenderCallback) override;

private:
	IRDevice*			m_pRenderer;

	long				m_CurrentFrame;
	CTimer				m_GlobalTimer;
};