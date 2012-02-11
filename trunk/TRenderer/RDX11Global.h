#pragma once

#include "IEngine.h"

#include "RDefine.h"
#include "IShader.h"
#include "IRenderStateMgr.h"
#include "IRDX11Device.h"
#include "IRenderTargetMgr.h"
#include "IFontRenderer.h"


namespace GLOBAL
{
	IEngine*					Engine();

	ID3D11Device*				D3DDevice();
	ID3D11DeviceContext*		D3DContext();

	const RDeviceDesc&			DeviceInfo();
	const CCAMERA_DESC*			CameraDesc();
	void						SetCameraDesc(CCAMERA_DESC* pDesc);

	IRDX11Device*				RDevice();
	IRenderStateMgr*			RenderStateMgr();
	IShaderMgr*					ShaderMgr();
	IRenderHelper*				RenderHelper();
	IRenderTargetMgr*			RenderTargetMgr();
	IFontRenderer*				FontRenderer();

	void						Present();
	bool						Resize(int width, int height);

	bool						StartUp(const CENGINE_INIT_PARAM* pParam, IEngine* pEngine);
	void						ShutDown();
}