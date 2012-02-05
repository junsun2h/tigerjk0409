#include "RDX11Global.h"
#include "RDX11RenderStrategeDeffered.h"


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
	GLOBAL::RenderTargetMgr()->ClearAndSetMaineFrame();

	IShader* pShader = GLOBAL::ShaderMgr()->GetShader(EFFECT_GPASS_LAMBERT);
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
