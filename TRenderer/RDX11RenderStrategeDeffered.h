#pragma once

#include "IRDevice.h"


class RDX11RenderStrategeDeffered : public IRenderStrategy
{
	void		Render(const CCAMERA_DESC& cameraDesc) override;

public:
	void		SkinPass();
	void		ShadowPass();
	void		GeometryPass();
	void		LightPass();
	void		SSAOPass();
	void		MaterialPass();
};