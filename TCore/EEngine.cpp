#include <windows.h>

#include "CDefine.h"
#include "CTimer.h"
#include "CLog.h"
#include "CEngineParam.h"
#include "CCamera.h"

#include "IEntity.h"
#include "IEntityProxy.h"
#include "IEntityMgr.h"
#include "IAssetMgr.h"
#include "ILoader.h"
#include "ISpaceMgr.h"
#include "IRDevice.h"
#include "IActorMgr.h"
#include "IRenderCommand.h"

#include "EGlobal.h"
#include "EEngine.h"



EEngine::EEngine()
	: m_pRenderer(NULL)
	, m_CurrentFrame(0)
{
	m_GlobalTimer.Start();
}

IRDevice*			EEngine::RDevice()			{ return m_pRenderer;}
IAssetMgr*			EEngine::AssetMgr()			{ return GLOBAL::AssetMgr(); }
IEntityMgr*			EEngine::EntityMgr()		{ return GLOBAL::EntityMgr(); }
IRenderHelper*		EEngine::RenderHelper()		{ return m_pRenderer->GetRenderHelper(); }
CTimer*				EEngine::GlobalTimer()		{ return &m_GlobalTimer; }
ILoader*			EEngine::Loader()			{ return GLOBAL::Loader(); }
ISpaceMgr*			EEngine::SpaceMgr()			{ return GLOBAL::SpaceMgr(); }
long				EEngine::GetCurrentFrame()	{ return m_CurrentFrame; }


bool EEngine::StartUp(const CENGINE_INIT_PARAM* pParam)
{
	//////////////////////////////////////////////////////////////////////////
	// load Render DLL
	HMODULE	renderDll = ::LoadLibrary( L"Renderer.dll");
	if( renderDll == NULL )
	{
		assert(0);
		return false;
	}

	typedef IRDevice *(*CREATE_RENDERER)();
	CREATE_RENDERER FuncCreateRenderer = (CREATE_RENDERER)::GetProcAddress( renderDll, "CreateDX11Renderer" );

	if( FuncCreateRenderer == NULL )
		return false;

	m_pRenderer = FuncCreateRenderer();
	m_pRenderer->StartUp( pParam, this );

	GLOBAL::AsyncRenderer()->InitAsyncRenderThreadObjects();

	//////////////////////////////////////////////////////////////////////////
	// initialize Asset manager
	GLOBAL::Loader()->Init( pParam->numOfProcessThread );
	GLOBAL::SpaceMgr()->Init( 10000, 10);
	CLOG::InitLogSystem();

	return true;
}

bool EEngine::ShutDown()
{
	// make sure deleting order
	GLOBAL::AssetMgr()->Clear();
	m_pRenderer->ShutDown();
	GLOBAL::EntityMgr()->Destroy();
	GLOBAL::SpaceMgr()->Destroy();
	return false;
}


void EEngine::Resize(UINT width, UINT height)
{
	m_pRenderer->Resize(width, height);
}


void EEngine::UpdateAndRender(CCAMERA_DESC* pCameraDesc, IRenderingCallback* pRenderCallback)
{
	float deltaTime = m_GlobalTimer.GetElapsedTime();
	m_GlobalTimer.CountFPS(deltaTime);

	//////////////////////////////////////////////////////////////////////////
	// 1) update Render independent system


	//////////////////////////////////////////////////////////////////////////
	// 2) update culled space list
	GLOBAL::SpaceMgr()->UpdateVisibleSpaceList( pCameraDesc );

	//////////////////////////////////////////////////////////////////////////
	// 3) update render dependent system
	GLOBAL::ActorMgr()->Update(deltaTime);

	GLOBAL::AsyncRenderer()->AsyncRender( pCameraDesc , pRenderCallback);

	m_CurrentFrame++;
}