#pragma once


class RDX11RenderStrategeForward : public IRenderStrategy
{
public:
	RDX11RenderStrategeForward();

	void			RenderFrame(CCAMERA_DESC* pCameraDesc) override;
	void			Render(CRenderElement* pRenderElement) override;
	void			RenderGeometry(CResourceGeometry* pGeometry) override;
};