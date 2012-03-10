#include "CDefine.h"
#include "CResource.h"
#include "CCamera.h"
#include "RDefine.h"
#include "CRenderElement.h"
#include "CLight.h"

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
	GLOBAL::ShaderMgr()->SetShaderConstant( &cameraConstant, sizeof( CCAMERA_DESC), 12, PIXEL_SHADER );

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
	sunDesc.ambientColor = CVector3(0.4f, 0.4f, 0.4f);
	GLOBAL::ShaderMgr()->SetShaderConstant( &sunDesc, sizeof( SunBuffer), 11, PIXEL_SHADER );

	GLOBAL::RenderTargetMgr()->ClearAndSetMaineFrame();
}

void RDX11RenderStrategeForward::Render(CRenderElement* pRenderElement)
{
	IShaderMgr* pShaderMgr = GLOBAL::ShaderMgr();

	pRenderElement->pPixelShader->Begin();
	pRenderElement->pVertexShader->Begin();

	{
		struct TModelVS
		{
			XMMATRIX wvp;
			XMMATRIX wv;
		}modelVS;

		const CCAMERA_DESC* pCamera = GLOBAL::CameraDesc();

		modelVS.wv = XMMatrixMultiply( pRenderElement->worldMatrix, pCamera->ViewTM ); 
		modelVS.wvp = XMMatrixMultiply( modelVS.wv, pCamera->ProjTM ); 

		modelVS.wv = XMMatrixTranspose( modelVS.wv );
		modelVS.wvp = XMMatrixTranspose( modelVS.wvp );

		pShaderMgr->SetShaderConstant( &modelVS, sizeof( TModelVS), 11, VERTEX_SHADER );
	}

	if( pRenderElement->material.pTextures[TEXTURE_DIFFISE] != NULL )
		pShaderMgr->SetTexture( pRenderElement->material.pTextures[TEXTURE_DIFFISE], 0);

	if( pRenderElement->material.pTextures[TEXTURE_BUMP] != NULL )
		pShaderMgr->SetTexture( pRenderElement->material.pTextures[TEXTURE_BUMP], 1);

	if( pRenderElement->pRefMatrix )
	{
		for(UINT i =0; i< pRenderElement->refMatrixCount; ++i)
			pRenderElement->pRefMatrix[i] = XMMatrixTranspose( pRenderElement->pRefMatrix[i] );

		GLOBAL::ShaderMgr()->SetShaderConstant( pRenderElement->pRefMatrix, 
												sizeof(XMMATRIX) * pRenderElement->refMatrixCount, 
												12, VERTEX_SHADER );
	}

	float buf[1024];
	buf[0] = float(pRenderElement->lightCount);
	UINT lightBufSize = 4;
	XMMATRIX wv = XMMatrixMultiply( pRenderElement->worldMatrix, GLOBAL::CameraDesc()->ViewTM ); 

	for( UINT i=0; i < pRenderElement->lightCount; ++i)
	{
		CVector3 lightViewPos = CVector3::Transform( pRenderElement->pLights[i].pos, wv);
		buf[lightBufSize++] = lightViewPos.x;
		buf[lightBufSize++] = lightViewPos.y;
		buf[lightBufSize++] = lightViewPos.z;
		buf[lightBufSize++] = pRenderElement->pLights[i].range;
		buf[lightBufSize++] = pRenderElement->pLights[i].color.x;
		buf[lightBufSize++] = pRenderElement->pLights[i].color.y;
		buf[lightBufSize++] = pRenderElement->pLights[i].color.z;
		buf[lightBufSize++] = pRenderElement->pLights[i].intensity;
	}

	pShaderMgr->SetShaderConstant( buf, 4 * lightBufSize, 10, VERTEX_SHADER);


	RenderGeometry(pRenderElement->pGeometry);
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