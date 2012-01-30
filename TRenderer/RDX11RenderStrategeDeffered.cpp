#include "RDX11RenderStrategeDeffered.h"
#include "RDX11Device.h"
#include "RDX11Miscellaneous.h"


void RDX11RenderStrategeDeffered::RenderScene()
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
	GLOBAL::GetMainFrameRenderTarget()->ClearAndSet();

	RDX11Shader* pShader = GLOBAL::GetShaderMgr()->GetShader(EFFECT_GPASS_LAMBERT);
	
	pShader->Begin();
	GLOBAL::Engine()->SpaceMgr()->Render();
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
