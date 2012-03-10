#include <windows.h>

#include "CDefine.h"
#include "CTimer.h"
#include "CLog.h"
#include "CEngineParam.h"
#include "CCamera.h"
#include "CResource.h"
#include "CLight.h"

#include "IEntity.h"
#include "IEntityProxy.h"
#include "IEntityMgr.h"
#include "IAssetMgr.h"
#include "ILoader.h"
#include "ISpaceMgr.h"
#include "IRDevice.h"
#include "IActorMgr.h"
#include "IRenderer.h"
#include "IRenderHelper.h"
#include "ILightMgr.h"

#include "EGlobal.h"
#include "EEngine.h"


namespace GLOBAL
{
	extern IRDevice*	g_pRenderer;
};



EEngine::EEngine()
	: m_CurrentFrame(0)
{
	m_GlobalTimer.Start();
}

IAssetMgr*			EEngine::AssetMgr()			{ return GLOBAL::AssetMgr(); }
IEntityMgr*			EEngine::EntityMgr()		{ return GLOBAL::EntityMgr(); }
CTimer*				EEngine::GlobalTimer()		{ return &m_GlobalTimer; }
ILoader*			EEngine::Loader()			{ return GLOBAL::Loader(); }
ISpaceMgr*			EEngine::SpaceMgr()			{ return GLOBAL::SpaceMgr(); }
long				EEngine::GetCurrentFrame()	{ return m_CurrentFrame; }
IRenderer*			EEngine::Renderer()			{ return GLOBAL::Renderer(); }
ILightMgr*			EEngine::LightMgr()			{ return GLOBAL::LightMgr(); }


bool EEngine::StartUp(const CENGINE_INIT_PARAM* pParam)
{
	//////////////////////////////////////////////////////////////////////////
	// load Render DLL
#ifdef _DEBUG
	HMODULE	renderDll = ::LoadLibrary( L"Rendererd.dll");
#else
	HMODULE	renderDll = ::LoadLibrary( L"Renderer.dll");
#endif

	if( renderDll == NULL )
	{
		assert(0);
		return false;
	}

	typedef IRDevice *(*CREATE_RENDERER)();
	CREATE_RENDERER FuncCreateRenderer = (CREATE_RENDERER)::GetProcAddress( renderDll, "CreateDX11Renderer" );

	if( FuncCreateRenderer == NULL )
		return false;

	GLOBAL::g_pRenderer = FuncCreateRenderer();
	GLOBAL::g_pRenderer->StartUp( pParam );

	CResourceTexture* pTexture = (CResourceTexture*)GLOBAL::Loader()->Load( "Data\\font\\Font.dds", "fontTexture", RESOURCE_FILE_TEXTURE, true);

	GLOBAL::RDevice()->GetRenderHelper()->SetFontTexture( pTexture);
	GLOBAL::Renderer()->Init();

	//////////////////////////////////////////////////////////////////////////
	// initialize Asset manager
	GLOBAL::Loader()->Init( pParam->numOfProcessThread );
	GLOBAL::SpaceMgr()->Init( 10000, 10);
	CLOG::InitLogSystem();
	
	return true;
}

bool EEngine::ShutDown()
{
	GLOBAL::Renderer()->Destroy();

	// make sure deleting order
	GLOBAL::LightMgr()->Destroy();
	GLOBAL::AssetMgr()->Clear();
	GLOBAL::RDevice()->ShutDown();
	GLOBAL::EntityMgr()->Destroy();
	GLOBAL::SpaceMgr()->Destroy();
	return false;
}


void EEngine::Resize(UINT width, UINT height)
{
	GLOBAL::RDevice()->Resize(width, height);
}


void EEngine::UpdateAndRender(CCAMERA_DESC* pCameraDesc, IRenderingCallback* pRenderCallback)
{
	float deltaTime = m_GlobalTimer.GetElapsedTime();
	m_GlobalTimer.CountFPS(deltaTime);

	//////////////////////////////////////////////////////////////////////////
	// 1) update Render independent system
	GLOBAL::Loader()->CompleteWork(1000);

	//////////////////////////////////////////////////////////////////////////
	// 2) update culled space list
	GLOBAL::SpaceMgr()->UpdateVisibleSpaceList( pCameraDesc );

	//////////////////////////////////////////////////////////////////////////
	// 3) update render dependent system
	GLOBAL::ActorMgr()->Update(deltaTime);

	GLOBAL::Renderer()->AsyncRender( pCameraDesc , pRenderCallback);

	m_CurrentFrame++;
}