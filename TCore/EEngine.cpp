#include "EEngine.h"
#include "EEntityProxyCamera.h"
#include "IRDevice.h"
#include "CLog.h"
#include <windows.h>



EEngine::EEngine()
	: m_pRenderer(NULL)
	, m_CurrentFrame(0)
{
	m_GlobalTimer.Start();
}

EEngine::~EEngine()
{

}

bool EEngine::StartUp(const CENGINE_INIT_PARAM &param)
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
	m_pRenderer->StartUp( param, this );

	//////////////////////////////////////////////////////////////////////////
	// initialize Asset manager
	m_Loader.Init( param.numOfProcessThread );
	m_QuadSpaceMgr.Init( 10000, 10);
	CLOG::InitLogSystem();

	return true;
}

bool EEngine::ShutDown()
{
	m_QuadSpaceMgr.Destroy();
	m_AssetMgr.Clear();
	m_pRenderer->ShutDown();
	m_EntityMgr.Destroy();

	return false;
}


void EEngine::Resize(UINT width, UINT height)
{
	m_pRenderer->Resize(width, height);
}


void EEngine::UpdateAndRender(IEntityProxyCamera* pCamera, IRenderingCallback* pRenderCallback)
{
	float deltaTime = m_GlobalTimer.GetElapsedTime();

	//////////////////////////////////////////////////////////////////////////
	// 1) update Render independent system


	//////////////////////////////////////////////////////////////////////////
	// 2) update culled space list
	m_QuadSpaceMgr.UpdateVisibleSpaceList(pCamera);

	//////////////////////////////////////////////////////////////////////////
	// 3) update render dependent system

	//////////////////////////////////////////////////////////////////////////
	// 4) render current frame
	if( pRenderCallback )
		pRenderCallback->PreRender();

	m_pRenderer->RenderFrame( pCamera->GetDesc() );

	if( pRenderCallback )
		pRenderCallback->PostRender();

	m_pRenderer->Present();

	m_CurrentFrame++;
}