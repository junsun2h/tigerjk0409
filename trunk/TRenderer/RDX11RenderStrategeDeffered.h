#pragma once


class RDX11RenderStrategeDeffered : public IRenderStrategy
{
	void		RenderScene() override;

public:
	void		SkinPass();
	void		ShadowPass();
	void		GeometryPass();
	void		LightPass();
	void		SSAOPass();
	void		MaterialPass();
};