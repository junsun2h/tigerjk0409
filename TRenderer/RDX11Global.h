#pragma once

#include "RDefine.h"

#include "IRDevice.h"
#include "IRenderTargetMgr.h"

struct IRenderStateMgr;
struct IShader;
struct IShaderMgr;


namespace GLOBAL
{
	ID3D11Device*				D3DDevice();
	ID3D11DeviceContext*		D3DContext();

	const RDeviceDesc&			DeviceInfo();
	const CCAMERA_DESC*			CameraDesc();
	void						SetCameraDesc(CCAMERA_DESC* pDesc);

	IRDevice*					RDevice();
	IRenderStateMgr*			RenderStateMgr();
	IShaderMgr*					ShaderMgr();
	IRenderHelper*				RenderHelper();
	IRenderTargetMgr*			RenderTargetMgr();

	void						Present();
	bool						Resize(int width, int height);

	bool						StartUp(const CENGINE_INIT_PARAM* pParam);
	void						ShutDown();
}