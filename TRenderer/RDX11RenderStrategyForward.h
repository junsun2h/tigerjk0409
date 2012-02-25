#pragma once


class RDX11RenderStrategeForward : public IRenderStrategy
{
public:
	RDX11RenderStrategeForward();

	void			RenderFrame(CCAMERA_DESC* pCameraDesc) override;
	void			RenderGeometry(CResourceGeometry* pGeometry) override;

	void			SetMaterial(const CResourceMtrl* pMaterial, const CResourceGeometry* pGeometry) override;

	void			SetTransform( const XMMATRIX& worldTM ) override;
	void			SetJointTransforms( XMMATRIX* pJointTM, UINT size ) override;
};