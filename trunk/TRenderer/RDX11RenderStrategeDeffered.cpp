#include "RDX11RenderStrategeDeffered.h"


void RDX11RenderStrategeDeffered::Render(const CCAMERA_DESC& cameraDesc)
{
	// pre-render Target Drawing
	SkinPass();
	ShadowPass();

	// Main Render Target Drawing
	GeometryPass();
	LightPass();

	// After Effect
	SSAOPass();
	MaterialPass();
}

void RDX11RenderStrategeDeffered::SkinPass()
{

}

void RDX11RenderStrategeDeffered::ShadowPass()
{

}

void RDX11RenderStrategeDeffered::GeometryPass()
{

}

void RDX11RenderStrategeDeffered::LightPass()
{

}

void RDX11RenderStrategeDeffered::SSAOPass()
{

}

void RDX11RenderStrategeDeffered::MaterialPass()
{

}
