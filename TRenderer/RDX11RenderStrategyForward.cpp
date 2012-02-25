#include "CDefine.h"
#include "CResource.h"
#include "CCamera.h"
#include "RDefine.h"

#include "IRDevice.h"
#include "IShader.h"
#include "IRenderStateMgr.h"

#include "RDX11Global.h"
#include "RDX11RenderStrategyForward.h"

RDX11RenderStrategeForward::RDX11RenderStrategeForward()
{

}

void RDX11RenderStrategeForward::RenderFrame(CCAMERA_DESC* pCameraDesc)
{
	CCAMERA_DESC desc;

	desc = *pCameraDesc;
	desc.ProjTM._33 /= desc.farClip;
	desc.ProjTM._43 /= desc.farClip;

	GLOBAL::SetCameraDesc(&desc);

	// update global camera info
	CCAMERA_DESC cameraConstant = desc;
	cameraConstant.ViewTM = XMMatrixTranspose( desc.ViewTM );
	cameraConstant.ProjTM = XMMatrixTranspose( desc.ProjTM );
	GLOBAL::ShaderMgr()->UpdateShaderConstant( &cameraConstant, sizeof( CCAMERA_DESC), 12, PIXEL_SHADER );

	// update global sun light info
	struct SunBuffer
	{
		CVector3	direction;
		float		intensity;
		CVector3	ambientColor;
		float		ambientIntensity;
	}sunDesc;

	sunDesc.direction = CVector3::TransformNormal( CVector3(1,1,1), pCameraDesc->ViewTM );
	sunDesc.direction = CVector3::Normalize(sunDesc.direction);
	sunDesc.ambientColor = CVector3(0.1f, 0.f, 0.f);
	GLOBAL::ShaderMgr()->UpdateShaderConstant( &sunDesc, sizeof( SunBuffer), 11, PIXEL_SHADER );

	GLOBAL::RenderTargetMgr()->ClearAndSetMaineFrame();
}

void RDX11RenderStrategeForward::RenderGeometry(CResourceGeometry* pGeometry)
{
	if( pGeometry->loadState != RESOURCE_LOAD_FINISHED )
	{
		assert(0);
		return;
	}

	GLOBAL::RenderStateMgr()->SetVertexInput( pGeometry->eVertexType );
	GLOBAL::RenderStateMgr()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT offset[1] = { 0 };
	UINT stride[1];

	if( pGeometry->pGraphicMemoryVertexBufferOut != NULL)
	{
		stride[0] = VERTEX_STRIDE(pGeometry->eVertexType) + 4;
		GLOBAL::D3DContext()->IASetVertexBuffers( 0, 1, (ID3D11Buffer**)&pGeometry->pGraphicMemoryVertexBufferOut, stride, offset );
	}
	else
	{
		stride[0] = VERTEX_STRIDE(pGeometry->eVertexType);
		GLOBAL::D3DContext()->IASetVertexBuffers( 0, 1, (ID3D11Buffer**)&pGeometry->pGraphicMemoryVertexBuffer, stride, offset );
	}

	if( pGeometry->pIndexBuffer != NULL)
	{
		if( pGeometry->eIndexType == INDEX_16BIT_TYPE )
			GLOBAL::D3DContext()->IASetIndexBuffer( (ID3D11Buffer*)pGeometry->pGraphicMemoryIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
		else
			GLOBAL::D3DContext()->IASetIndexBuffer( (ID3D11Buffer*)pGeometry->pGraphicMemoryIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

		GLOBAL::D3DContext()->DrawIndexed( pGeometry->primitiveCount * 3, 0, 0 );
	}
}

void RDX11RenderStrategeForward::SetMaterial(const CResourceMtrl* pMaterial, const CResourceGeometry* pGeometry)
{
	//1. decide Effect
	//2. set Shader
	//3. set material params to device

	IShaderMgr* pShaderMgr = GLOBAL::ShaderMgr();
	if( pMaterial == NULL)
	{
		if( pGeometry->IsSkinedMesh() )
			pShaderMgr->GetShader(GPASS_VS_LAMBERT_WEIGHT)->Begin();
		else
			pShaderMgr->GetShader(GPASS_VS_LAMBERT)->Begin();

		pShaderMgr->GetShader(GPASS_PS_LAMBERT)->Begin();
	}
}

void RDX11RenderStrategeForward::SetTransform( const XMMATRIX& worldTM )
{	
	GLOBAL::ShaderMgr()->GetCurrentVS()->SetShaderContants(worldTM);
}

void RDX11RenderStrategeForward::SetJointTransforms( XMMATRIX* pJointTM, UINT size )
{	
	GLOBAL::ShaderMgr()->GetCurrentVS()->SetShaderContants( pJointTM, size);
}
