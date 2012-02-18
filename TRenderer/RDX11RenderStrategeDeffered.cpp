#include "CDefine.h"
#include "CResource.h"
#include "CCamera.h"
#include "RDefine.h"

#include "IRDevice.h"
#include "ISpaceMgr.h"
#include "IShader.h"
#include "IRenderStateMgr.h"

#include "RDX11Global.h"
#include "RDX11RenderStrategeDeffered.h"

RDX11RenderStrategeDeffered::RDX11RenderStrategeDeffered()
	: m_currentRenderPass(OPAQUE_PASS)
{

}

void RDX11RenderStrategeDeffered::RenderFrame(CCAMERA_DESC* pCameraDesc)
{
	CCAMERA_DESC desc;

	desc = *pCameraDesc;
	desc.ProjTM._33 /= desc.farClip;
	desc.ProjTM._43 /= desc.farClip;

	GLOBAL::SetCameraDesc(&desc);

	// update global shader constant
	CCAMERA_DESC cameraConstant = desc;
	cameraConstant.ViewTM = XMMatrixTranspose( desc.ViewTM );
	cameraConstant.ProjTM = XMMatrixTranspose( desc.ProjTM );
	GLOBAL::ShaderMgr()->UpdateShaderConstant( &cameraConstant, sizeof( CCAMERA_DESC), SM_BUF12_192BYTE_SLOT1, PIXEL_SHADER );

	GLOBAL::RenderTargetMgr()->ClearAndSetMaineFrame();

	OpaquePass();
	TransparentPass();
}

void RDX11RenderStrategeDeffered::OpaquePass()
{
	m_currentRenderPass = OPAQUE_PASS;
	GLOBAL::Engine()->SpaceMgr()->Render();
}

void RDX11RenderStrategeDeffered::TransparentPass()
{
	m_currentRenderPass = TRANSPARENT_PASS;
}

void RDX11RenderStrategeDeffered::RenderGeometry(CResourceGeometry* pGeometry)
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

void RDX11RenderStrategeDeffered::SetMaterial(const CResourceMtrl* pMaterial)
{
	//1. decide Effect
	//2. set Shader
	//3. set material params to device

	IShaderMgr* pShaderMgr = GLOBAL::ShaderMgr();
	if( pMaterial == NULL)
	{
		pShaderMgr->GetShader(GPASS_VS_LAMBERT)->Begin();
		pShaderMgr->GetShader(GPASS_PS_LAMBERT)->Begin();
	}
}

void RDX11RenderStrategeDeffered::SetTransform( const XMMATRIX& worldTM )
{	
	IShader* pShader = GLOBAL::ShaderMgr()->GetCurrentVS();
	pShader->SetShaderContants(worldTM);
}